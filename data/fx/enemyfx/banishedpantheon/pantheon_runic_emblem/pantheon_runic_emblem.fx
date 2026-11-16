#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Hair
End

Input  
	Inpname	Root
End

Input  
	Inpname	Emblem
End

Flags    InheritAlpha


#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	PantheonNode
		Type	Local 
		At	Emblem
		GEOM	Chest_Rune_Pivot
		Sound rune_loop 20 20 .75
	
	End
End

#########################################################
########  CHEST RUNES  ##################################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	PantheonNode
		AltPiv	1
		Part	:Pantheon_Runic_Emblem1.part

	End
End

Condition
	On 	Time
	Time 	100

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	PantheonNode
		AltPiv	1
		Part	:Pantheon_Runic_Emblem2.part
	
	End
End


Condition
	On 	Time
	Time 	200

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	PantheonNode
		AltPiv	1
		Part	:Pantheon_Runic_Emblem3.part
	
	End
End

Condition
	On 	Time
	Time	300

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	PantheonNode
		AltPiv	1
		Part	:Pantheon_Runic_Emblem4.part
	
	End
End

#########################################################
########  CHEST GLOW   ##################################
#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	PantheonNode
		AltPiv	1
		Part	:Pantheon_Runic_Glow1.part
		Part	:Pantheon_Runic_Glow2.part
	End
End

End