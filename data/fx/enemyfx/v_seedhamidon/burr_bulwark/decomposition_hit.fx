#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Decomposition_Hit_01 100 100 .8
	End

	Event
		Ename	Hit
		Type	Local
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hit
		Type	Local
		At	Root
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
		At	Chest
		Part	:Decomp_HitSplatter_Mult.part
		Part	:Decomp_HitSplatter_Mult2.part
		Part	:Decomp_Splatter_Spray.part
		Lifespan 5
	End


End

############################################

#
#	Event
#		Ename	FireBallHit
#		Type	Posit
#		At	Hit
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Part	:Decomp_HitFlash.part
#		Part	:Decomp_HitFlash_Ground.part
#		Part	:Decomp_Impact_Alpha.part
#		Part	:Decomp_Impact_Mult.part
#		Part	:Decomp_Impact_Alpha_out.part
#		Part	:Decomp_Impact_Mult_out.part
#		Lifespan 30
#	End
#End

#############################################################

End