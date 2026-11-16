#############################################################
## Child_OilPuddles.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Cycle
	Time	5
	Chance	0.5

	Event
		Ename	OilDrips
		Type 	Posit
		At	Origin
		Until	Air
		BhvrOverride
			PositionOffset	0 0.2 0.5
		End
		part	:Puddle_Trail.part
		part	:Puddle_Trail_Long.part
		Lifespan	4
		LifespanJitter	2
	End
End

#############################################################
End