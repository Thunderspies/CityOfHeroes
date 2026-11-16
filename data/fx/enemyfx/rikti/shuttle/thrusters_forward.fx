#########################################################
## Rikti Transport Shuttle - Thrusters Forward FX
#########################################################

FxInfo

## SOUND FX #######################################################

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound thruster_loop 500 500 .5
	End

End

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound arachnoflyer_loop 500 500 .5
	End

End

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound policebot1_loop 500 500 .5
	End

End


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound arachnosflyerbeep_loop 400 400 .18
	End

End

## STARBOARD ENGINE #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	UArmR
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmR
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End
End

## PORT ENGINE #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	UArmL
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmL
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End
End

#########################################################

End		