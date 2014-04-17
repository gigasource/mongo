/*-
 * Copyright (c) 2008-2014 WiredTiger, Inc.
 *	All rights reserved.
 *
 * See the file LICENSE for redistribution information.
 */

#include "wt_internal.h"
/*
 * __async_get_key --
 *	WT_ASYNC_OP->get_key implementation for op handles.
 */
static int
__async_get_key(WT_ASYNC_OP *asyncop, ...)
{
	WT_DECL_RET;
	va_list ap;

	va_start(ap, asyncop);
	ret = __wt_cursor_get_keyv(&asyncop->c, asyncop->c.flags, ap);
	va_end(ap);
	return (ret);
}

/*
 * __async_set_key --
 *	WT_ASYNC_OP->set_key implementation for op handles.
 */
static void
__async_set_key(WT_ASYNC_OP *asyncop, ...)
{
	va_list ap;

	va_start(ap, asyncop);
	__wt_cursor_set_keyv(&asyncop->c, asyncop->c.flags, ap);
	__wt_buf_set(O2S((WT_ASYNC_OP_IMPL *)asyncop), &asyncop->c.key,
	    asyncop->c.key.data, asyncop->c.key.size);
	va_end(ap);
}

/*
 * __async_get_value --
 *	WT_ASYNC_OP->get_value implementation for op handles.
 */
static int
__async_get_value(WT_ASYNC_OP *asyncop, ...)
{
	WT_DECL_RET;
	va_list ap;

	va_start(ap, asyncop);
	ret = __wt_cursor_get_valuev(&asyncop->c, ap);
	va_end(ap);
	return (ret);
}

/*
 * __async_set_value --
 *	WT_ASYNC_OP->set_value implementation for op handles.
 */
static void
__async_set_value(WT_ASYNC_OP *asyncop, ...)
{
	va_list ap;

	va_start(ap, asyncop);
	__wt_cursor_set_valuev(&asyncop->c, ap);
	__wt_buf_set(O2S((WT_ASYNC_OP_IMPL *)asyncop), &asyncop->c.value,
	    asyncop->c.value.data, asyncop->c.value.size);
	va_end(ap);
}

/*
 * __async_op_wrap --
 *	Common wrapper for all async operations.
 */
static int
__async_op_wrap(WT_ASYNC_OP_IMPL *op, WT_ASYNC_OPTYPE type)
{
	op->optype = type;
	return (__wt_async_op_enqueue(O2C(op), op));
}

/*
 * __async_search --
 *	WT_ASYNC_OP->search implementation for op handles.
 */
static int
__async_search(WT_ASYNC_OP *asyncop)
{
	WT_ASYNC_OP_IMPL *op;
	WT_DECL_RET;
	WT_SESSION_IMPL *session;

	op = (WT_ASYNC_OP_IMPL *)asyncop;
	ASYNCOP_API_CALL(O2C(op), session, search);
	WT_STAT_FAST_CONN_INCR(O2S(op), async_op_search);
	WT_ERR(__async_op_wrap(op, WT_AOP_SEARCH));
err:	API_END(session);
	return (ret);
}

/*
 * __async_insert --
 *	WT_ASYNC_OP->insert implementation for op handles.
 */
static int
__async_insert(WT_ASYNC_OP *asyncop)
{
	WT_ASYNC_OP_IMPL *op;
	WT_DECL_RET;
	WT_SESSION_IMPL *session;

	op = (WT_ASYNC_OP_IMPL *)asyncop;
	ASYNCOP_API_CALL(O2C(op), session, insert);
	WT_STAT_FAST_CONN_INCR(O2S(op), async_op_insert);
	WT_ERR(__async_op_wrap(op, WT_AOP_INSERT));
err:	API_END(session);
	return (ret);
}

/*
 * __async_update --
 *	WT_ASYNC_OP->update implementation for op handles.
 */
static int
__async_update(WT_ASYNC_OP *asyncop)
{
	WT_ASYNC_OP_IMPL *op;
	WT_DECL_RET;
	WT_SESSION_IMPL *session;

	op = (WT_ASYNC_OP_IMPL *)asyncop;
	ASYNCOP_API_CALL(O2C(op), session, update);
	WT_STAT_FAST_CONN_INCR(O2S(op), async_op_update);
	WT_ERR(__async_op_wrap(op, WT_AOP_UPDATE));
err:	API_END(session);
	return (ret);
}

/*
 * __async_remove --
 *	WT_ASYNC_OP->remove implementation for op handles.
 */
static int
__async_remove(WT_ASYNC_OP *asyncop)
{
	WT_ASYNC_OP_IMPL *op;
	WT_DECL_RET;
	WT_SESSION_IMPL *session;

	op = (WT_ASYNC_OP_IMPL *)asyncop;
	ASYNCOP_API_CALL(O2C(op), session, remove);
	WT_STAT_FAST_CONN_INCR(O2S(op), async_op_remove);
	WT_ERR(__async_op_wrap(op, WT_AOP_REMOVE));
err:	API_END(session);
	return (ret);
}

/*
 * __async_get_id --
 *	WT_ASYNC_OP->get_id implementation for op handles.
 */
static uint64_t
__async_get_id(WT_ASYNC_OP *asyncop)
{
	return (((WT_ASYNC_OP_IMPL *)asyncop)->unique_id);
}

/*
 * __async_op_init --
 *	Initialize all the op handle fields.
 */
static int
__async_op_init(WT_CONNECTION_IMPL *conn, WT_ASYNC_OP_IMPL *op, uint32_t id)
{
	WT_ASYNC_OP *asyncop;

	asyncop = (WT_ASYNC_OP *)op;
	asyncop->connection = (WT_CONNECTION *)conn;
	asyncop->key_format = asyncop->value_format = NULL;
	asyncop->c.key_format = asyncop->c.value_format = NULL;
	asyncop->get_key = __async_get_key;
	asyncop->get_value = __async_get_value;
	asyncop->set_key = __async_set_key;
	asyncop->set_value = __async_set_value;
	asyncop->search = __async_search;
	asyncop->insert = __async_insert;
	asyncop->update = __async_update;
	asyncop->remove = __async_remove;
	asyncop->get_id = __async_get_id;
	/*
	 * The cursor needs to have the get/set key/value functions initialized.
	 * It also needs the key/value related fields set up.
	 */
	asyncop->c.get_key = __wt_cursor_get_key;
	asyncop->c.set_key = __wt_cursor_set_key;
	asyncop->c.get_value = __wt_cursor_get_value;
	asyncop->c.set_value = __wt_cursor_set_value;
	asyncop->c.recno = 0;
	memset(&asyncop->c.raw_recno_buf, 0, sizeof(asyncop->c.raw_recno_buf));
	memset(&asyncop->c.key, 0, sizeof(asyncop->c.key));
	memset(&asyncop->c.value, 0, sizeof(asyncop->c.value));
	asyncop->c.session = (WT_SESSION *)conn->default_session;
	asyncop->c.saved_err = 0;
	asyncop->c.flags = 0;

	op->internal_id = id;
	op->state = WT_ASYNCOP_FREE;
	return (0);
}

/*
 * __wt_async_op_enqueue --
 *	Enqueue an operation onto the work queue.
 */
int
__wt_async_op_enqueue(WT_CONNECTION_IMPL *conn, WT_ASYNC_OP_IMPL *op)
{
	WT_ASYNC *async;
	WT_DECL_RET;
	uint64_t my_alloc, my_slot;

	async = conn->async;
	/*
	 * Enqueue op at the tail of the work queue.
	 */
	WT_ASSERT(conn->default_session, op->state == WT_ASYNCOP_READY);
	/*
	 * We get our slot in the ring buffer to use.
	 */
	WT_WRITE_BARRIER();
	my_alloc = WT_ATOMIC_ADD(async->alloc_head, 1);
	my_slot = my_alloc % async->async_qsize;
	WT_ASSERT(conn->default_session, async->async_queue[my_slot] == NULL);
	async->async_queue[my_slot] = op;
	op->state = WT_ASYNCOP_ENQUEUED;
	WT_WRITE_BARRIER();
	if (WT_ATOMIC_ADD(async->cur_queue, 1) > async->max_queue)
		async->max_queue = async->cur_queue;
	/*
	 * Multiple threads may be adding ops to the queue.  We need to wait
	 * our turn to make our slot visible to workers.
	 */
	while (async->head != (my_alloc - 1))
		__wt_yield();
	WT_PUBLISH(async->head, my_alloc);
	return (ret);
}

/*
 * __wt_async_op_init --
 *	Initialize all the op handles.
 */
int
__wt_async_op_init(WT_CONNECTION_IMPL *conn)
{
	WT_ASYNC *async;
	WT_ASYNC_OP_IMPL *op;
	uint32_t i;

	async = conn->async;
	/*
	 * Initialize the flush op structure.
	 */
	__async_op_init(conn, &async->flush_op, OPS_INVALID_INDEX);

	/*
	 * Allocate and initialize the work queue.  This is sized so that
	 * the ring buffer is known to be big enough such that the head
	 * can never overlap the tail.  Include extra for the flush op.
	 */
	async->async_qsize = conn->async_size + 2;
	WT_RET(__wt_calloc_def(conn->default_session,
	    async->async_qsize, &async->async_queue));
	/*
	 * Allocate and initialize all the user ops.
	 */
	WT_RET(__wt_calloc_def(conn->default_session,
	    conn->async_size, &async->async_ops));
	for (i = 0; i < conn->async_size; i++) {
		op = &async->async_ops[i];
		__async_op_init(conn, op, i);
	}
	return (0);
}
