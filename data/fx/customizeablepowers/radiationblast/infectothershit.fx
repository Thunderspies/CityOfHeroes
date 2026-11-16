#############################################################
## InfectOthersHit.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	rays
		Type	Local
		At	Origin
		Part1	:InfectOthersHITRAYS.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	start
		At	Origin
		part2	:Radiationflash.part
		part3	:Radiationsmoke.part
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	StemSmoke
		Type	start
		At	Origin
		Part1	:InfectOthersStemSmoke.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0.0 11.0 0.0
		End
		Part1	:InfectOthersPlumeSmoke.part
		Lifespan 13
	End
End

#############################################################

Condition
	On	Time
	Time	11

	Event
		EName	RingsOffset01
		Type	start
		At	Origin
		bhvr    Behaviors\UpWardsMotion2.bhvr
	End

	Event
		Type	Local
		At	RingsOffset01
		Part1	:InfectOthersRing.part
		Lifespan 2
	End
End

Condition
	On	Time
	Time	21

	Event
		EName	RingsOffset02
		Type	start
		At	Origin
		bhvr    Behaviors\UpWardsMotion2.bhvr
	End

	Event
		Type	Local
		At	RingsOffset02
		Part1	:InfectOthersRing.part
		Lifespan 2
	End
End

#############################################################

End