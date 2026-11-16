#############################################################
## FrozenAuraAttack.fx
#############################################################

FxInfo
Lifespan	150

#############################################################

Condition

	On Time
	Time 0

	Event
		Type	PositOnly
		At	UlegR
		Sound IceNova2 100.0 100.0 .88
		Part	:Leg_Snow_Trail.part
		LifeSpan	30
	End

	Event
		Type	PositOnly
		At	LlegR
		Part	:Leg_Snow_Trail.part
		Part	:Shiver_Snowburst.part
		LifeSpan	30
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Leg_Snow_Trail.part
		Part	:Shiver_Snowburst.part
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	StartPositOnly
		At	FootR
		Bhvr	Behaviors/CameraShake01.bhvr
	End

	Event
		EName   RadiusFxScale
		Type	StartPositOnly
		At	FootR
		Bhvr	powers/firecontrol/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		Bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part	:FrozenAura_Shockwave.part
		Part	:FrozenAura_IceCrystals.part
		Part	:FrozenAura_IceCrystals2.part
		Part	:Shiver_SnowBurst.part
	End

	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		Bhvr    behaviors/powers/firecontrol/Infernobubble2.bhvr
		Part	:FrozenAura_SnowExplosion.part

	End

	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		Bhvr    behaviors/powers/firecontrol/Infernobubble3.bhvr
		Part	:FrozenAura_Shockwave.part
	End

	Event
		EName	Ring
		Type	StartPositOnly
		At	RadiusFxScale
		Geom	Tintable_MegaRing2
		Bhvr	behaviors/IceAuraRing.bhvr
		LifeSpan	3

	End
End

#############################################################

Condition
	On 	Time
	Time	67

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End
