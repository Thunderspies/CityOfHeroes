#############################################################
## Barrage_Hit.fx
#############################################################

FxInfo
LifeSpan 70

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	ChestAnchor
		Type	Local
		At	Chest
		Sound LambdaTurret_Barrage_Hit_01 300 300 .8
		Lifespan	20
	End
End

Condition
	On 	Cycle
	Time 	5

	Event
		Type	Local
		At	ChestAnchor
		BhvrOverride
			StartJitter	1.0 1.0 0.0
		End
		Part	:Flash_Circle_Small.part
		Part	:Smoke_Circle.part
		Part	:Flash_Small.part
		Part	:Sparks_Hit.part
		Lifespan	2
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Head
		Part	:Body_Steam.part
		POther	Neck
		Lifespan 	40
		LifespanJitter	10

	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Chest
		Part	:Body_Steam.part
		POther	Hips
		Lifespan 	40
		LifespanJitter	10
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Body_Steam.part
		POther	LarmL
		Lifespan 	40
		LifespanJitter	10
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Body_Steam.part
		POther	LArmR
		Lifespan 	40
		LifespanJitter	10
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Body_Steam.part
		POther	HandL
		Lifespan 	40
		LifespanJitter	10
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Body_Steam.part
		POther	HandR
		Lifespan 	40
		LifespanJitter	10
	End
End

#############################################################

End