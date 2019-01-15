
#ifndef _HEADER_IRQ
#define _HEADER_IRQ

void init_IRQ(void);
void init_traps(void);

struct irq_chip {
	const char		*name;
	unsigned int	(*irq_startup)(struct irq_data *data);
	void	(*irq_shutdown)(struct irq_data *data);
	void	(*irq_enable)(struct irq_data *data);
	void	(*irq_disable)(struct irq_data *data);

	void	(*irq_ack)(struct irq_data *data);
	void	(*irq_mask)(struct irq_data *data);
	void	(*irq_mask_ack)(struct irq_data *data);
	void	(*irq_unmask)(struct irq_data *data);
	void	(*irq_eoi)(struct irq_data *data);

	void	(*irq_cpu_online)(struct irq_data *data);
	void	(*irq_cpu_offline)(struct irq_data *data);

	void	(*irq_print_chip)(struct irq_data *data, struct seq_file *p);

	unsigned long	flags;
};


#endif
