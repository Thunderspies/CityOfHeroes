#############################################################
##	Fireweapon
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		#Sound	firemotor1_loop 30 20 .22
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound firebeep1 30 30 .26
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	WaterBackPack
		Type	Local
		At	chest
		Bhvr	:WaterbackPackFade.bhvr
		Geom	WaterBackPackRed
	End

	Event
		Type	Local
		At	WaterBackPack
		altpiv	1
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:BackPackLight.part
		Part	:BackPackLightGlowstar.part
		Part	:BackPackLightGlowstar2.part
		#LifeSpan	2
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WaterBackPack
		altpiv	1
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:BackPackLight.part
		Part	:BackPackLightGlowstar.part
		Part	:BackPackLightGlowstar2.part
		#LifeSpan	2
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	WaterBackPack
		altpiv	2
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:BackPackLight.part
		Part	:BackPackLightGlowstar.part
		Part	:BackPackLightGlowstar2.part
		#LifeSpan	2
	End
End

Condition
	On 	Time
	Time 	21

	Event
		Type	Local
		At	WaterBackPack
		altpiv	2
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:BackPackLight.part
		Part	:BackPackLightGlowstar.part
		Part	:BackPackLightGlowstar2.part
		#LifeSpan	2
	End
End

#############################################################

End