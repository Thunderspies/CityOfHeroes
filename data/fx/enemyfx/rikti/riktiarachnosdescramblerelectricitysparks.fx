#########################################################
##	RadiationAura
FxInfo

LifeSpan	100
###########################################################################################
#############################################################

Condition
		
	Event
		Ename	Prime
		Type	local
		At	origin
		
		LifeSpan	10
		LifeSpanJitter	10

	End

End

Condition
	On	PrimeDied
	Random	3
		
	Event
		Type	local
		At	LlegL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	LarmR
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	ULegL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	LarmR
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	LLegL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	LLegR
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	ULegL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	ULegR
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	Hips
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	WepL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End
	
	Event
		Type	local
		At	WepR
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	60
	End

End


End				