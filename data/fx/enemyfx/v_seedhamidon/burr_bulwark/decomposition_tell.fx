#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	FireBallHit
		Type	Posit
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Decomp_Ground_Glow.part
		Part	:Decomp_Ground_Alpha.part
		Part	:Decomp_Ground_Add.part
		Part	:Decomp_GasRise_Target.part
		#Part	:Decomp_GasRise_Random.part
		Lifespan 250
	End
End

#############################################################

End