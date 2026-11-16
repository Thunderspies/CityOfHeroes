#############################################################
## ChillingHands.fx
#############################################################

FxInfo

Lifespan 60

#############################################################
Condition
	On 	Time
	Time	2
	Event
		Type	Local
		At	Root
		Sound 	WB_Whirlpool 100 100 .8
	End
End

Condition
	On	Time
	Time	0

		Event
			At WepL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 20
		End

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 20
		End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	WepL
		Part1	POWERS\ColdControl\FreezingTouchRing.part
		Part 	:WaterShot_Splash_Initial01.part
		Part 	:WaterShot_Splash_Initial02.part
		Part 	:WaterShot_Splash_Trail_01.part
		Part 	:WaterShot_Splash_Trail_02.part
		Part 	:WaterShot_Splash_Trail_03.part
		Part 	:WaterShot_Splash_Trail_04.part
	End

	Event
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\FreezingTouchRing.part
		Part 	:WaterShot_Splash_Initial01.part
		Part 	:WaterShot_Splash_Initial02.part
		Part 	:WaterShot_Splash_Trail_01.part
		Part 	:WaterShot_Splash_Trail_02.part
		Part 	:WaterShot_Splash_Trail_03.part
		Part 	:WaterShot_Splash_Trail_04.part
	End
End

#############################################################

End