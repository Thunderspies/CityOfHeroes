#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Malignant_ContinuingChild_01 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_02 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_03 100 100 .8
	End

#	Event
#		ENAME	HeadGlow
#		Type	Local
#		At	Hair
#		Part	:Splatter_Tumors.part
#		part	:Glow_Tumors.part
#		POther  Chest
#	End
#
#	Event
#		ENAME	ChestGlow
#		Type	Local
#		At	Chest
#		part	:Glow_Tumors.part
#		ChildFX	:CHILD_splatter.fx
#		POther  Hips
#	End

	Event
		Ename	FireBallHit
		Type	Posit
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Decomp_HitFlash.part
		Part	:Decomp_HitFlash_Ground.part
		Part	:Decomp_Impact_Alpha.part
		Part	:Decomp_Impact_Mult.part
		Part	:Decomp_Impact_Alpha_out.part
		Part	:Decomp_Impact_Mult_out.part
		Lifespan 30
	End


End


#############################################################

End