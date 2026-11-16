#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Root
		Sound Emote_Confetti 200 200 1
		# Sound Emote_ConfettiThrow 200 200 1
	End

	Event
		Type	Start
		At	Root
		Part	:ConfettiSparkles_Purple.part
		Part	:ConfettiSparkles_Yellow.part
		ChildFX	:BodyGlow.fx

		LifeSpan 	5

	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0 -1.0 0
		End
		Part	:Sparks.part
		Part	:Sparks_Vertical.part
		Lifespan	5
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Start
		At	Root
		LifeSpan 	5
		Part	:SmokeAdditive.part

	End

End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	Chest
		LifeSpan 	5
		Part	:SmokeLight.part

	End

	Event
		Type	Start
		At	Root
		LifeSpan 	5
		Part	:SmokeLight2.part
	End
End

#############################################################


Condition
	On	Time
	Time	1
#############################################################

	Event
		Ename	Rocket1
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner1
		Type	Local
		At	Rocket1
		BhvrOverride
			Spin 0.0 .5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset1
		Type 	Local
		At	Spinner1
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part1	:BottleRocketSparks.part
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 15
	End

#############################################################

	Event
		Ename	Rocket2
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Rocket2
		BhvrOverride
			Spin 0.0 -.5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset2
		Type 	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part1	:BottleRocketSparks.part
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 17
	End

#############################################################

	Event
		Ename	Rocket3
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner3
		Type	Local
		At	Rocket3
		BhvrOverride
			Spin 0.0 .5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset3
		Type 	Local
		At	Spinner3
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part1	:BottleRocketSparks.part
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 19
	End

#############################################################

End