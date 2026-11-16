#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFX	:Tumors.fx
		Lifespan 	80
	End

End

Condition
	On 	Time
	Time 	70

	Event	Type	Local
		At	Root
		ChildFX	:Tumors_Pop.fx
	End

#############################################################

	Event
		Type	Local
		At	Chest
		Part	:PusExplosion_Goo.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Hips
		Part	:Sparks.part
		Lifespan 5
	End

#############################################################

	Event
		Type	StartPositOnly
		At	Hips
		ChildFX	:PoisonSplash.fx
	End
End

#############################################################

Condition
	On	Time
	Time	71

	Event
		Type	Local
		At	UArmR
		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	UArmL
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	LLegR
		Part	:PusExplosion_GooLite.part
		Part	:PusDripping2.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	LLegL
		Part	:PusExplosion_Goo.part
		Part	:PusExplosion_GooLite.part
		Lifespan 	50
		LifespanJitter	15
	End
End

#############################################################

Condition
	On	Time
	Time	72

	Event
		Type	Local
		At	WepR
		Part	:PusExplosion_Goo.part
		Part	:PusDripping2.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	WepL
		Part	:PusExplosion_Goo.part
		Part	:PusDripping.part
		Lifespan 	50
		LifespanJitter	15
	End
End

#############################################################

Condition
	On 	Time
	Time 	72



	Event
		Type	Local
		At	SpadL
		POther	LArmL

		Part	:PusExplosion.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	SpadR
		POther	LArmR

		Part	:PusExplosion.part
		Lifespan 	50
		LifespanJitter	15
	End

	Event
		Type	Local
		At	WepL
		Part	:PusExplosion.part
	End

	Event
		Type	Local
		At	WepR
		Part	:PusExplosion.part
	End

#############################################################

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\Splat\DarkOilLarge.bhvr
		BhvrOverride
			Scale		12.0 0.5 12.0
			ScaleRate	8.0 0.0 8.0
			EndScale	22.0 0.5 22.0

			FadeInLength 		15

			Alpha		255
			StartColor	230 255 80
		End
		Splat	DarknessSplat_White.tga
		Lifespan 	50
		LifespanJitter	15
	End
End

#############################################################

Condition
	On	Time
	Time	74

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:PusExplosion_Goo_Splash.part
		Lifespan 	20
	End
End

#############################################################

End

