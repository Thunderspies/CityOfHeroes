#############################################################
## Aura_HellOnEarth.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Root
		Part	:Heal_Shockwave.part
		Part	:Heal_Shockwave_Highlight.part
		Part	:AbyssalMend_FirePillar.part
		Part	:AbyssalMend_FirePillar_Alpha.part

	End
End

Condition

	On 	Cycle
	Time 	15
	Chance	60

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.3 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.0 1.0 1.0
 			EndScale		0.5 0.5 0.5
		End
		Part	:AbyssalMend_FireRing_Origin.part
		Part	:AbyssalMend_FireRing_Flat.part
		Part	:AbyssalMend_FireRing_Small.part
		Part	:AbyssalMend_FireRing_Small_Alpha.part

		Part	:Sparks_Hellfire_Big.part
		Lifespan 60
	End
End


#############################################################

End


