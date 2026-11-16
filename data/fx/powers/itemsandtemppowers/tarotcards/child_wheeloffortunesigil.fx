#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Bhvr	:Splat_WheelOfFortune.bhvr
		Splat	Splat_WheelOfFortune
		Lifespan 58
	End

	Event
		EName 	SigilPulse
		Type	Local
		At	Root
		part	:WheelOfFortuneSigil_Pulse1.part
		Lifespan 75
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	RedStreaks_OutterRing
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0 0.1 0
		End
	End

	Event
		Type	Local
		At	RedStreaks_OutterRing
		BhvrOverride
			PositionOffset	0.0 0.0 2.5
		End
		part	:WheelOfFortuneSigil_BlueStreaks1.part
		Lifespan	50
		LifespanJitter	10
	End

	Event
		Type	Local
		At	RedStreaks_OutterRing
		BhvrOverride
			PositionOffset	0.0 0.0 -2.5
		End
		part	:WheelOfFortuneSigil_BlueStreaks1.part
		Lifespan	50
		LifespanJitter	10
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	RedStreaks_InnerRing
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0 -0.1 0
		End
	End

	Event
		Type	Local
		At	RedStreaks_InnerRing
		BhvrOverride
			PositionOffset	0.0 0.0 1.75
		End
		part	:WheelOfFortuneSigil_BlueStreaks1.part
		Lifespan	50
		LifespanJitter	10
	End

	Event
		Type	Local
		At	RedStreaks_InnerRing
		BhvrOverride
			PositionOffset	0.0 0.0 -1.75
		End
		part	:WheelOfFortuneSigil_BlueStreaks1.part
		Lifespan	50
		LifespanJitter	10
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	BlueStreaks_InnerRing
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0 0.1 0
		End
	End

	Event
		Type	Local
		At	BlueStreaks_InnerRing
		BhvrOverride
			PositionOffset	0.0 0.0 0.5
		End
		part	:WheelOfFortuneSigil_RedStreaks1.part
		Lifespan	50
		LifespanJitter	10
	End
End

#############################################################

End