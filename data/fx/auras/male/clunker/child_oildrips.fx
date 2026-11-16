#############################################################
## Child_OilPuddles.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Cycle
	Time	50
	Chance	0.3

	Event
		Ename	OilDrips
		Type 	Local
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Oil_Strands.part
		Lifespan	10
		LifespanJitter	5
	End
End

#############################################################
End