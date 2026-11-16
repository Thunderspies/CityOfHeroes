#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

#Lifespan 500

############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Feather_Falloff_Trail.part
		Part	:Feather_Falloff_Idle.part
	End

	Event
		Type	Local
		At	UArmL
		Part	:Feather_Falloff_Idle.part
	End

	Event
		Type	Local
		At	UArmR
		Part	:Feather_Falloff_Idle.part
	End

End

Condition
	On	Cycle
	Time	20
	Event
		Ename	BigFeather
		At	Chest
		Type	Start

		ChildFX	:CHILD_BigFeathers_Steady.fx
		Lifespan 100
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FORWARD
	DoMany		1

	Event
		Ename	BigFeather
		At	Chest
		Type	Local
		Flags	OneAtATime
		While	FORWARD

		ChildFX	:CHILD_BigFeathers.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	BACKWARD
	DoMany		1

	Event
		Ename	BigFeather
		At	Chest
		Type	Local
		Flags	OneAtATime
		While	BACKWARD

		ChildFX	:CHILD_BigFeathers.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	STEPLEFT
	DoMany		1

	Event
		Ename	BigFeather
		At	Chest
		Type	Local
		Flags	OneAtATime
		While	BACKWARD

		ChildFX	:CHILD_BigFeathers.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	STEPRIGHT
	DoMany		1

	Event
		Ename	BigFeather
		At	Chest
		Type	Local
		Flags	OneAtATime
		While	BACKWARD

		ChildFX	:CHILD_BigFeathers.fx
	End
End

#############################################################

End