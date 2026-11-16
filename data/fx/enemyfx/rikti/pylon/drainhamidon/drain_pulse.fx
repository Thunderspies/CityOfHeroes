#########################################################
## Rikti Portal Stabilizer - Large Beam
########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	PylonOffset
		Type	Local
		At 	Chest
		bhvroverride
			positionoffset	0.0 8.0 0.0
			pyrrotate	0.0 0.0 0.0
		End
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type	Start
		At	Origin
		Bhvr	:Pulse.bhvr
		Part1	:Pulse1.part
		Part1	:Pulse2.part
		Part1	:Pulse3.part
		Magnet	PylonOffset
		LookAt	PylonOffset
	End		
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		EName 	Prime
	End
End

########################################################

End