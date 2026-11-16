#############################################################
## Giant Spider - Glowing Eyes
#############################################################

FxInfo

Flags InheritAlpha

## EYES ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.4 2.39 2.175
		End
		Part1	:GiantSpider_EyeGlow_01.part
		Sound TarantulaEyes_loop 50 50 .36
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.39 2.44 2.09
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.37 2.49 1.99
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.34 2.53 1.91
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.4 2.39 2.175
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.39 2.44 2.09
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.37 2.49 1.99
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.34 2.53 1.91
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End
End

## BODY ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.8 2.043 2.13
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		0.87 2.11 2.03
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.75 1.76 2.02
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		-0.93 2.53 1.53
		End
		Part1	:GiantSpider_EyeGlow_01.part
	End
End

#############################################################

End