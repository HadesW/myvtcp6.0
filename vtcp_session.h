#ifndef VTCP_SESSION_H
#define VTCP_SESSION_H

#include "vtcp.h"
#include "vtcp_queue.h"
#include "vtcp_packet.h"

#include <stdio.h>
#include <tchar.h>

#define VTCP_SESSION_FLAG_WORKING									0x01

struct vtcp_session
{
	//标识(虚拟句柄)
	// 本地
	unsigned short index0;
	// 远程
	unsigned short index1;

	//状态
	unsigned char state;
	//标志
	unsigned char flags;

	unsigned int last_send;
	unsigned int last_recv;

	// 地址
	unsigned char address[20];

	// 发送数据列表
	struct vtcp_queue queue1;
	// 接收包
	struct vtcp_packet packet0;
	// 发送包
	struct vtcp_packet packet1;

	void *pcs;

	int	errorcode;							//内部错误代码

											//序号发生器
	unsigned int sn;

	//发送序号底端(被确认序号)
	unsigned int minimum1;
	//发送序号高端(最大许可序号)
	unsigned int maximum1;
	//发送序号当前(发送但未确认序号)
	unsigned int current1;
	//发送序号更新(序号)
	unsigned int update;

	//接收序号底端(确认序号但未传输给客户)
	unsigned int minimum0;
	//接收序号高端(最大许可序号)
	unsigned int maximum0;
	//接收序号当前(确认序号)
	unsigned int current0;

	//TCP-RTT测量
	unsigned int rtt;							//发包往返周期（RTT）
	unsigned int rtt_prev;						//发包往返周期（RTT）

												//TCP发包速度控制
	unsigned long long	send_data_speed_surplus;		//发包速度剩余（65536余）
	unsigned long long	send_data_speed;				//发包速度（65536倍）每毫秒
	unsigned long long	send_data_speed_prev;			//发包速度（65536倍）每毫秒
	unsigned long long	send_data_speed_change;		//发包速度变化量
	unsigned long long	send_data_speed_change_prev;	//发包速度变化量
	int send_data_speed_level;		//发包速度水平（0~15）

										// 重发个数
	unsigned int repeat;
	unsigned int send_count;
	// 发包拥塞窗口
	unsigned int cwnd_prev_prev;
	// 发包拥塞窗口
	unsigned int cwnd_prev;
	// 发包拥塞窗口
	unsigned int cwnd;

	// TCP回包速度控制
	// 接收数据包个数
	unsigned int recv_count;
	// 接收数据包回应频率
	unsigned int recv_ack_freq;

	// public://TCP带宽预测
	// 	int		m_tcp_recv_data_series_speed;		
	// 	int		m_tcp_recv_data_series_space;
	// 	int		m_tcp_recv_data_series_count;
	// 	int		m_tcp_recv_data_series_count_temp;
	// 	int		m_tcp_recv_data_series_time0;
	// 	int		m_tcp_recv_data_series_time1;
	// 	int		m_tcp_recv_data_series_time2;

	//TCP参数
	//逗留
	unsigned int linger;
	// 逗留时间
	unsigned int linger_timeout;
	//逗留(内部使用)
	unsigned int linger_timeout_tick;
	// 活动时间
	unsigned int keepalive;
	// 活动时间间隔
	unsigned int keepalive_internal;
	// 连接超时
	unsigned int connect_timeout;
	// 连接超时(内部使用)
	unsigned int connect_timeout_tick;

	//统计变量
	unsigned long long count_do_send_data;
	unsigned long long count_do_send_data_ack;
	unsigned long long count_do_send_data_ack_lost;
	unsigned long long count_do_send_data_repeat;
	unsigned long long count_do_send_sync;
	unsigned long long count_do_send_sync_ack;
	unsigned long long count_do_send_reset;
	unsigned long long count_do_send_reset_ack;

	//统计变量
	unsigned long long count_on_recv_data;
	unsigned long long count_on_recv_data_ack;
	unsigned long long count_on_recv_data_ack_lost;
	unsigned long long count_on_recv_sync;
	unsigned long long count_on_recv_sync_ack;
	unsigned long long count_on_recv_reset;
	unsigned long long count_on_recv_reset_ack;

	//统计速度
	unsigned long long count_recv;
	unsigned long long count_send;
	unsigned long long count_send_bytes;
};

struct vtcp_session *vtcp_connect(struct vtcp *pvtcp, const unsigned char *address, unsigned int addresssize, unsigned int tickcount);
int vtcp_send(struct vtcp *pvtcp, struct vtcp_session *psession, const unsigned char *buffer, unsigned int length, unsigned int tickcount);

int vtcp_session_timer(struct vtcp *pvtcp, unsigned int tickcount, unsigned int delta, unsigned int count);

void vtcp_door_onrecv(struct vtcp *pvtcp, struct vtcp_pkt *pp, unsigned int pkt_size, const unsigned char *address, unsigned int addresssize, int errorcode, unsigned int tickcount);
int vtcp_onrecv(struct vtcp *pvtcp, struct vtcp_pkt *pp, unsigned int pkt_size, const unsigned char *address, unsigned int addresssize, int errorcode, unsigned int tickcount);


#endif