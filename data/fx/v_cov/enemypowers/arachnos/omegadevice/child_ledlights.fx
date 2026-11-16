#############################################################
## Omega Device - Child FX - LED Lights
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Test
		Type	local
		At 	origin
		BhvrOverride
			PositionOffset		0.0 0.0 -2.0
			Scale			0.25 0.25 0.25
		End
		Geom	GEO_FX_TestSphere
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	CoreNodez
		Type	local
		At 	origin
		BhvrOverride
			PyrRotate	0 -280 0
			PositionOffset	0.17 -.07 0.07
		End
		Geom	Led_Offset
	End

	Event
		Type	local
		At 	CoreNodez
		altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Red_LED.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	CoreNodey
		Type	local
		At 	origin
		BhvrOverride
			PyrRotate	0 -270 0
			PositionOffset	0.17 -.07 0.01
		End
		Geom	Led_Offset
	End

	Event
		Type	local
		At 	CoreNodey
		altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Red_LED_Small2.part
	End
End

Condition
	On 	Time
	Time 	0

	Event

		Ename	CoreNodex
		Type	local
		At 	origin
		BhvrOverride
			PyrRotate	0 -260 0
			PositionOffset	0.15 -.07 -0.02
		End
		Geom	Led_Offset
	End

	Event
		Type	local
		At 	CoreNodex
		altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Red_LED.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	CoreNodew
		Type	local
		At 	origin
		BhvrOverride
			PyrRotate	0 -259 0
			PositionOffset	0.23 -.07 -0.08
		End
		Geom	Led_Offset
	End

	Event

		Type	local
		At 	CoreNodew
		altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Red_LED_Small2.part
	End
End

#############################################################


End