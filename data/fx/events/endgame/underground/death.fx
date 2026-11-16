#############################################################
## Death.fx
#############################################################

FxInfo
Flags DontSuppress
Lifespan 310

########################################################



## POWER FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Arm
		Type	Local
		At	Origin

		ChildFX	:Death_Boils.fx
		Lifespan 	190
	End
End

################################################################

Condition

	On	Time
	Time	30

	Event
		EName	SplashEvent
		Type	Local
		At	Root
		CHILDFX	:Splash_Body.fx
		Lifespan 	90
	End
End

Condition
	On	Time
	Time	85

	Event
		EName	Shake
		Type	Local
		At	Root
		BhvrOverride
			Shake          1.0
			ShakeFallOff   .08
			ShakeRadius    180.0
			FadeOutLength	5
		End

		Lifespan 	5
	End

	Event
		Type	PositOnly
		At	LLegL
		Part	:Infection_Spores_Burst.part
		Part	:Infection_Dust_Burst_Fall.part
		Lifespan 	5
	End

	Event
		Type	PositOnly
		At	LLegR
		Part	:Infection_Spores_Burst.part
		Part	:Infection_Dust_Burst_Fall.part
		Lifespan 	5
	End
End

Condition
	On	Time
	Time	155

	Event
		EName	Shake2
		Type	Local
		At	Root
		BhvrOverride
			Shake          1.0
			ShakeFallOff   .03
			ShakeRadius    180.0
			FadeOutLength	5
		End
		Lifespan 	5
	End

	Event
		Type	PositOnly
		At	Neck
		Part	:Infection_Spores_Burst.part
		Part	:Infection_Dust_Burst_Fall2.part
		Lifespan 	5
	End
End


Condition

	On	Time
	Time	185

	Event
		EName 	Prime
		Type	start
		At	Head
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Shake          2.0
			ShakeFallOff   .05
			ShakeRadius    180.0
			FadeOutLength	5
		End
		Flags	AdoptParentEntity
		Part	:Death_Flash_Huge.part
		Part	:Death_Flash_Huge_Ground.part
		Part	:Death_Flash.part
		Part	:Death_Steam.part
		Part	:Death_Splash_Additive.part
		Part	:Death_Shockwave.part
		Part	:Death_Splash_Initial.part
		Lifespan	10
	End


End

Condition
	On	Time
	Time	190

	Event
		EName 	Prime
		Type	start
		At	Hips
		Part	:Death_Flash.part
		Part	:Death_Steam.part
		Part	:Death_Splash_Additive.part
		Part	:Death_Shockwave.part
		Part	:Death_Splash_Initial.part
		Lifespan	10
	End

	Event
		EName	Sigil
		Type	StartPositOnly
		At	Hips
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	0 200 220
			Scale		150.0 25.0 150.0
		End
		Splat	HamiSplatter.tga
		Lifespan 190
	End
End

Condition
	On	Time
	Time	185

	Event
		EName 	HipsAnchor
		Type	PositOnly
		At	Root
		Lifespan	45
	End

	Event
		Type	Local
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 90 30.0
		End
		Part	:Death_Rain.part
		Part	:Death_Rain_Fill.part
		Lifespan	70
	End

	Event
		Type	Local
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 1.0 30.0
			StartJitter	0.0 1.0 0.0
		End
		Part	:Death_Rain_Splatter.part
		Lifespan	110
	End

	Event
		Type	Local
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 1.0 30.0
		End
		Part	:Death_Rain_Ripples.part
		Part	:Death_Rain_Ripples_Fill.part

		Lifespan	110
	End
End

########################################################

End