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


#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	PantheonNode
		Type	Local 
		At	Emblem
		GEOM	Chest_Rune_Pivot_Husk
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
		Part	:Pantheon_Runic_Emblem1_Husk.part

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
		Part	:Pantheon_Runic_Emblem2_Husk.part
	
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
		Part	:Pantheon_Runic_Emblem3_Husk.part
	
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
		Part	:Pantheon_Runic_Emblem4_Husk.part
	
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
		Part	:Pantheon_Runic_Glow1_Husk.part
		Part	:Pantheon_Runic_Glow2_Husk.part
	End
End

End