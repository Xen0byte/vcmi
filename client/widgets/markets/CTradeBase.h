/*
 * CTradeBase.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include "TradePanels.h"

VCMI_LIB_NAMESPACE_BEGIN

class IMarket;
class CGHeroInstance;

VCMI_LIB_NAMESPACE_END

class CButton;
class CSlider;

class CTradeBase
{
public:
	const IMarket * market;
	const CGHeroInstance * hero;

	//all indexes: 1 = left, 0 = right
	std::array<std::vector<std::shared_ptr<CTradeableItem>>, 2> items;
	std::shared_ptr<TradePanelBase> bidTradePanel;
	std::shared_ptr<TradePanelBase> offerTradePanel;

	//highlighted items (nullptr if no highlight)
	std::shared_ptr<CTradeableItem> hLeft;
	std::shared_ptr<CTradeableItem> hRight;
	std::shared_ptr<CButton> deal;
	std::shared_ptr<CSlider> offerSlider;
	std::shared_ptr<CButton> maxAmount;

	std::vector<std::shared_ptr<CLabel>> labels;
	std::vector<std::shared_ptr<CTextBox>> texts;

	CTradeBase(const IMarket * market, const CGHeroInstance * hero);
	void removeItems(const std::set<std::shared_ptr<CTradeableItem>> & toRemove);
	void removeItem(std::shared_ptr<CTradeableItem> item);
	void getEmptySlots(std::set<std::shared_ptr<CTradeableItem>> & toRemove);
	virtual void makeDeal() = 0;
	virtual void deselect();
	virtual void onSlotClickPressed(const std::shared_ptr<CTradeableItem> & newSlot, std::shared_ptr<CTradeableItem> & hCurSlot);
	virtual void updateSlots() {};	// TODO make pure virtual
};

// Market subclasses
class CExperienceAltar : virtual public CTradeBase, virtual public CIntObject
{
public:
	std::shared_ptr<CLabel> expToLevel;
	std::shared_ptr<CLabel> expForHero;
	std::shared_ptr<CButton> sacrificeAllButton;
	const Point dealButtonPos = Point(269, 520);

	CExperienceAltar();
	virtual void sacrificeAll() = 0;
	virtual TExpType calcExpAltarForHero() = 0;
};

class CCreaturesSelling : virtual public CTradeBase, virtual public CIntObject
{
public:
	CCreaturesSelling();
	bool slotDeletingCheck(const std::shared_ptr<CTradeableItem> & slot);
	void updateSubtitle();
	void updateSlots() override;
};

class CResourcesBuying : virtual public CTradeBase, virtual public CIntObject
{
public:
	CResourcesBuying(TradePanelBase::UpdateSlotsFunctor callback);
	void updateSubtitles(EMarketMode marketMode);
};

class CResourcesSelling : virtual public CTradeBase, virtual public CIntObject
{
public:
	CResourcesSelling();
	void updateSlots() override;
};

class CMarketMisc : virtual public CTradeBase, virtual public CIntObject
{
public:
	struct SelectionParamOneSide
	{ 
		std::string text; 
		int imageIndex; 
	};
	using SelectionParams = std::tuple<std::optional<const SelectionParamOneSide>, std::optional<const SelectionParamOneSide>>;
	using SelectionParamsFunctor = std::function<SelectionParams()>;

	CMarketMisc(SelectionParamsFunctor callback);
	void deselect() override;
	void updateSelected();

protected:
	int bidQty;
	int offerQty;
	SelectionParamsFunctor selectionParamsCallback;
};
