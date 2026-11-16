#############################################################
## LightningBlast.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	Root
		Sound Emote_Lightning_01 120 120 .9
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Ename	CameraShake
		Type	local
		At	Root
		Bhvr	Behaviors\SSLSShake.bhvr
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Ename	MainBolt
		Type	Local
		At	Root
		ChildFX	:BlueLightning\Lightning_Blue.fx
	End
End

Condition
	On	Time
	Time	33

	Event
		ENAME	HeadGlow
		Type	Local
		At	Origin
		Bhvr	Behaviors/TelportationFade.bhvr
		Part	:LargeBolts.part
		Part	:LightningGlow.part
		Lifespan 10
		Pmagnet Chest
	End

	Event
		Type	Local
		At	Hips
#		Part	:SmallGlow.part
		Part	:LowerBlast.part
		Part	:LargeGlow.part
		Part	:SmokePillar3.part
	End
End

Condition
	On	Time
	Time 	33

	Event
		Type	Local
		At	Hips
		Part	:MainExplosion.part
	End
End

#############################################################

Condition
	On	Time
	Time 	15
	#Chance	.25

	Event
		Type	Local
		At	Hips
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	Hair
		Lifespan 75
	End

	Event
		Type	Local
		At	Chest
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	Hair
		Lifespan 75
	End

	Event
		Type	Local
		At	UArmR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmR
		Lifespan 75
	End

	Event
		Type	Local
		At	LArmR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	WepR
		Lifespan 75
	End

	Event
		Type	Local
		At	UArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmL
		Lifespan 75
	End

	Event
		Type	Local
		At	UArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmL
		Lifespan 75
	End

	Event
		Type	Local
		At	LArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	WepL
		Lifespan 75
	End

	Event
		Type	Local
		At	FootR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegR
		Lifespan 75
	End

	Event
		Type	Local
		At	FootR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegR
		Lifespan 75
	End

	Event
		Type	Local
		At	LLegR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	ULegR
		Lifespan 75
	End


	Event
		Type	Local
		At	FootL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegL
		Lifespan 75
	End

	Event
		Type	Local
		At	LLegL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	ULegL
		Lifespan 75
	End
End

#############################################################

End