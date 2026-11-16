#############################################################
## DualBlades - Small Hit (Child)
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type 	Local
		At	Origin
		Part1	:DualBlades_HitFlash1.part
		Part5	:DualBlades_HitStar1.part
		Part3	:DualBlades_SmallHitEmbers1.part
		Part4	:DualBlades_SmallHitEmbers1.part
		#Sound	SwordSlicer 60 25 .76
		Lifespan	5
	End
End

#############################################################

Condition
	On	Time 
	Time	0

	Event
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End

	Event
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkUP.fx
	End
End

## HARDWARE ONLY ###########################################################

Condition
	On	Time 
	Time	0

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End
End

## CHANCE TO GO OFF ###########################################################

Condition
	On	Time 
	Time	0
	Chance	0.5
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUTbig.fx
	End
End

Condition
	On	Time 
	Time	2
	Chance	0.75
	
	Event
		EName	Sparkout
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End
End

#############################################################

End