#########################################################
## Rikti Transport Shuttle - Thrusters Idle FX
#########################################################

FxInfo


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound arachnoflyer_loop 400 400 .33
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
		#Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:Thruster_Idle_Core1.part
		#Part	:Thruster_Idle_Glow1.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmR
		#Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:Thruster_Idle_Core1.part
		#Part	:Thruster_Idle_Glow1.part
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
		#Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:Thruster_Idle_Core1.part
		#Part	:Thruster_Idle_Glow1.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmL
		#Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:Thruster_Idle_Core1.part
		#Part	:Thruster_Idle_Glow1.part
	End
End

#########################################################

End		