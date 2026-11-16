#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 320

#############################################################

##	TEST SPHERE

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 5 0
#			Scale		1.7 1.7 1.7
#		End
#		geom	tintable_FX_ForceBubble
#	End
#End

########################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Decomposition_Start_01 100 100 .8
	End


	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		BhvrOverride
			Spin			0 0.8 0

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		ChildFX	:Child_Decomp_ToxicBeam.fx
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End


Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
#End
#
######################
#
#
#Condition
#	On 	Time
#	Time 	12

	Event
		Ename	Hit
		Type	Local
		At	T_Root
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		Type	Local
		At	T_Chest
		Part	:Decomp_HitSplatter_Mult.part
		Part	:Decomp_HitSplatter_Mult2.part
		Part	:Decomp_Splatter_Spray.part
		Lifespan 5
	End

	Event
		Ename	Continueing
		Type	Local
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFX	:Decomposition_Continuing.fx
	End
End

########################
Condition
	On 	Time
	Time 	250
	Event
		Type	Local
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFX	:Decomposition_Splatter_Big.fx
	End
End


#############################################################

End