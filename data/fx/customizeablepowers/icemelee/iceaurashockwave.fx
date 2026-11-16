#############################################################
## IceAuraShockWave.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	UlegR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Part	:snowburst.part
		Sound IceNova2 100.0 100.0 0.88
		Lifespan 30
	End

	Event
		Type	Local
		At	LlegR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Part	:snowburst.part
		Lifespan 30
	End

	Event
		Type	Local
		At	FootR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Part	:snowburst.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	mystic
		Bhvr	Behaviors\CameraShake01.bhvr
		Lifespan 42
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	mystic
		Bhvr	:Inferno.bhvr
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr	:Infernobubble.bhvr
		Part	:ColdMistAura.part
		Part	:swordFlame01.part
		Part	:SwordFlame02.part
		Part	:IceCrystals.part
		Part	:IceCrystals2.part
		Part	:SnowFlakes.part
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr	:Infernobubble2.bhvr
		Part	:IceAuraSnow.part
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr	:Infernobubble3.bhvr
		Part	:IceAuraBits.part
	End

	Event
		Type	start
		At	RadiusFxScale
		bhvr	:IceAuraRing.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_MegaRing2
		Lifespan 3
	End
End

#############################################################

End