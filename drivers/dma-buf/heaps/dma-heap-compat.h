#ifndef _DMA_HEAP_COMPAT_H
#define _DMA_HEAP_COMPAT_H

#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>

#ifndef for_each_sgtable_sg
#define for_each_sgtable_sg(sgt, sg, i) \
	for_each_sg((sgt)->sgl, sg, (sgt)->orig_nents, i)
#endif

#ifndef for_each_sgtable_page
#define for_each_sgtable_page(sgt, piter, pgoff) \
	for_each_sg_page((sgt)->sgl, piter, (sgt)->orig_nents, pgoff)
#endif

static inline size_t heap_page_size(struct page *page)
{
	return PAGE_SIZE << compound_order(page);
}

static inline int dma_map_sgtable(struct device *dev, struct sg_table *sgt,
				  enum dma_data_direction dir,
				  unsigned long attrs)
{
	int nents = dma_map_sg_attrs(dev, sgt->sgl, sgt->nents, dir, attrs);
	if (!nents)
		return -ENOMEM;
	sgt->nents = nents;
	return 0;
}

static inline void dma_unmap_sgtable(struct device *dev, struct sg_table *sgt,
				     enum dma_data_direction dir,
				     unsigned long attrs)
{
	dma_unmap_sg_attrs(dev, sgt->sgl, sgt->nents, dir, attrs);
}

static inline void dma_sync_sgtable_for_cpu(struct device *dev,
					     struct sg_table *sgt,
					     enum dma_data_direction dir)
{
	dma_sync_sg_for_cpu(dev, sgt->sgl, sgt->nents, dir);
}

static inline void dma_sync_sgtable_for_device(struct device *dev,
						struct sg_table *sgt,
						enum dma_data_direction dir)
{
	dma_sync_sg_for_device(dev, sgt->sgl, sgt->nents, dir);
}

#define page_size(page) heap_page_size(page)

#endif
