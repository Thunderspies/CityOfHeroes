#############################################################
## Omega Device - Persistent FX
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

##########  Blades  + puff of smoke when blades pop out   #######################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Blade_1
		Type	local
		At 	Hips
		Bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\DroneBladesScale.bhvr
		geom	drone_Blades01
	End

	Event
		Ename	Blade_2
		Type	local
		At 	Hips
		Bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\DroneBladesScale.bhvr
		geom	drone_Blades02
	End

#	Event
#		Ename	Red_lights_Bottom
#		Type	local
#		At 	Hips
#		childFx	:Child_LEDLights.fx
#	End
#
#	Event
#		Ename	Red_lights_Top
#		Type	local
#		At 	Hips
#		BhvrOverride
#			PositionOffset		0.0 -0.02 0.0
#		End
#		childFx	:Child_LEDLights.fx
#	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	OffsetTopLight1
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		0.4 0.6 0.45
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetTopLight2
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		-0.45 0.6 -0.425
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetTopLight3
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		-0.45 0.6 0.425

		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetTopLight4
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		0.425 0.6 -0.425
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	OffsetBottomLight1
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		0.4 -0.6 0.45
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetBottomLight2
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		-0.45 -0.6 -0.425
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetBottomLight3
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		-0.45 -0.6 0.425
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End

	Event
		Ename	OffsetBottomLight4
		Type	Local
		At 	Hips
		BhvrOverride
			PositionOffset		0.425 -0.6 -0.425
		End
		Part	:RedLED_Glow01.part
		Part	:RedLED_Core01.part
	End
End

#########################################################
## Building
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Hips
		Part1	Scripted\LordReclusePTTBuff\Buildup_FinalContractingRing1.part
		Part2	Scripted\LordReclusePTTBuff\Buildup_FinalContractingRing2.part
		LifeSpan 120
	End
End
Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	origin
		Sound Arachnos_crab_spider_soldier_omega_maneuver_target 300 300 1
		LifeSpan 120
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Start
		At	Hips
		Part2	Scripted\LordReclusePTTBuff\Buildup_HitRaysBig.part
		lifespan 60
	End
End

Condition
	On	Time
	Time	90

	Event
		Type	Start
		At	Hips
		Part2	Scripted\LordReclusePTTBuff\Buildup_HitRaysBig.part
		Part4	Scripted\LordReclusePTTBuff\Buildup_HitStreaksBig.part
		lifespan 30
	End
End

Condition
	On	Time
	Time	105

	Event
		Type	Start
		At	Chest
		Part3	Scripted\LordReclusePTTBuff\Buildup_DiffuseGlow.part
		Part2	Scripted\LordReclusePTTBuff\Buildup_AdditiveGlow.part
		LifeSpan 15
	End
End

#############################################################

End