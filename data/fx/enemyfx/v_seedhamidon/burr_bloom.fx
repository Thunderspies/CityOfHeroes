#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

Lifespan 90

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0

	Event
		Ename	RadiusFxScale
		Type	Local
		At	T_Chest
		Flags	AdoptParentEntity
		bhvr    behaviors/Braz/ExpandRingTo30.bhvr
		BhvrOverride
			ScaleRate		0.1 0.1 0.1
 			EndScale		3 3 3
			FadeOutLength		20
		End
		Part	:BurrBloom_Tick.part
		Part	:BurrBloom_Dust.part
		#Part	:BurrBloom_Dust_Add.part
		Part	:BurrBloom_Globs.part
		Part	:BurrBloom_Border.part
		Lifespan 30
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End
#########################################################

End