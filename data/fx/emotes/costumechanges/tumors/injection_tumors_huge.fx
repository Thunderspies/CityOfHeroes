#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################

Condition
	On	Time
	Time	12

	Event
		EName	NodeSpin
		Type	Local
		At	WepR

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumPos.bhvr
		Geom	GEO_BoostSerum
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFX	:Tumors_Huge.fx
		Lifespan 	60
	End

End


Condition
	On 	Time
	Time 	60

	Event	Type	Local
		At	Root
		ChildFX	:BodyGlow_Tumors.fx
	End

	Event
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Head
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:PusExplosion2.part
		Part	:PusExplosion_GooLite.part
	End

	Event
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Hips
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:Sparks.part

	End


	Event
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	LLegL
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
	End

	Event
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	LLegR
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
	End
End

Condition
	On	Time
	Time	60
	Repeat 	3

	Event
		Type	StartPositOnly
		At	Hips
		ChildFX	:PoisonSplash.fx
	End
End

Condition
	On 	Time
	Time 	62

	Event
		Type	Local
		At	Chest
		POther	Hips

		Part	:PusExplosion_Goo.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	SpadL
		POther	LArmL

		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	SpadR
		POther	LArmR

		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	WepL
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:PusExplosion2.part
	End

	Event
		Type	Local
		At	WepR
		Part	:PusExplosion_Additive.part
		Part	:PusExplosion.part
		Part	:PusExplosion2.part

	End

	Event
		Type	Local
		At	LLegL
		Pother	Hips

		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15

	End


	Event
		Type	Local
		At	LLegR
		Pother	Hips
		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End


	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\Splat\DarkOilLarge.bhvr
		BhvrOverride
			Scale		20.0 20.0 20.0
			EndScale	20.0 20.0 20.0

			FadeInLength 		15

			Alpha		255
			StartColor	230 200 80
		End
		Splat	DarknessSplat.tga
		Lifespan 	50
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	64

	Event
		Type	Local
		At	WepL
		POther	LArmL
		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping.part
		Lifespan 	40
		LifespanJitter	15
	End

	Event
		Type	Local
		At	WepR
		POther	LArmR
		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping.part
		Lifespan 	40
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	69

	Event
		Type	StartPositOnly
		At	Root
		Part	:PusExplosion_Goo_Splash.part
		Lifespan 	9
	End

#############################################################

End

