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
	Inpname	Col_R
End

Input  
	Inpname	Col_L
End

Input  
	Inpname	Emblem
End

Input  
	Inpname	UArmR
End

Input  
	Inpname	UArmL
End

Input  
	Inpname	ULegR
End

Input  
	Inpname	ULegL
End

Input  
	Inpname	Hair
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Head
End

Flags    InheritAlpha

#########################################################
###### HEAD GLOWS #####################################
#########################################################


Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Pantheon_Head_Node
		Type	Local 
		At	Head
		GEOM	Pantheon_Head_Node
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	PantheonEyes
		Type	Local 
		At	Pantheon_Head_Node
		AltPiv	1
		Part	:PantheonGiantHull_Glow1.part
		Part	:GiantPantheonEyesMistRight.part

	End
	Event
		EName 	PantheonEyes
		Type	Local 
		At	Pantheon_Head_Node
		AltPiv	2
		Part	:PantheonGiantHull_Glow1.part
		Part	:GiantPantheonEyesMistLeft.part

	End

	Event
		EName 	PantheonEyes
		Type	Local 
		At	Pantheon_Head_Node
		AltPiv	3
		#Part	:PantheonGiantHull_Glow1.part
		#Part	:GiantPantheonEyesMistLeft.part

	End

End

#########################################################
###### CHEST GLOWS #####################################
#########################################################

Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Pantheon_Chest_Node
		Type	Local 
		At	Chest
		GEOM	Pantheon_Chest_Node
	
	End

End


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	ChestGlow1
		Type	Local 
		At	Pantheon_Chest_Node
		AltPiv	1
		#Part	:PantheonGiantHull_Glow2.part
	
	End
End

#########################################################
###### CHEST RUNES  #####################################
#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	Pantheon_Chest_Node
		Type	Local 
		At	Emblem
		GEOM	Pantheon_Chest_Node

	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	Pantheon_Chest_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune5.part

	End
End

Condition
	On 	Time
	Time 	100

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	Pantheon_Chest_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune6.part
	
	End
End


Condition
	On 	Time
	Time 	200

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	Pantheon_Chest_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune7.part
	
	End
End

Condition
	On 	Time
	Time	300

	Event
		EName 	PantheonRunicEmblem
		Type	Local 
		At	Pantheon_Chest_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune8.part
	
	End
End


#########################################################
###### UArmR Runic Glow  #####################################
#########################################################

Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Pantheon_Col_R_Node
		Type	Local 
		At	Col_R
		GEOM	Pantheon_Col_R_Node
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	ColRRune1
		Type	Local 
		At	Pantheon_Col_R_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune1.part
		Part	:PantheonGiantHull_Rune2.part
		BHVR	:GiantHullRunespin4.bhvr

	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	ColRRune1
		Type	Local 
		At	Pantheon_Col_R_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune3.part
		Part	:PantheonGiantHull_Rune4.part
		BHVR	:GiantHullRunespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	ColRRune1
		Type	Local 
		At	Pantheon_Col_R_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune1.part
		Part	:PantheonGiantHull_Rune4.part
		BHVR	:GiantHullRunespin6.bhvr
	
	End

End

#########################################################
###### UArmL Runic Glow  #####################################
#########################################################


Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Pantheon_Col_L_Node
		Type	Local 
		At	Col_L
		GEOM	Pantheon_Col_L_Node
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	ColLRune1
		Type	Local 
		At	Pantheon_Col_L_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune1.part
		Part	:PantheonGiantHull_Rune2.part
		BHVR	:GiantHullRunespin4.bhvr

	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	ColLRune1
		Type	Local 
		At	Pantheon_Col_L_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune3.part
		Part	:PantheonGiantHull_Rune4.part
		BHVR	:GiantHullRunespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	ColLRune1
		Type	Local 
		At	Pantheon_Col_L_Node
		AltPiv	1
		Part	:PantheonGiantHull_Rune1.part
		Part	:PantheonGiantHull_Rune4.part
		BHVR	:GiantHullRunespin6.bhvr
	
	End

End


#########################################################
###### ULegR Runic Glow  #####################################
#########################################################

#Condition
#	On 	Time  
#	Time 	0
	
#	Event
#		EName 	Pantheon_UlegR_Node
#		Type	Local 
#		At	ULegR
		#GEOM	Pantheon_UlegR_Node
	
#	End

#End

#Condition
#	On 	Time
#	Time 	0
	
#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegR_Node
#		AltPiv	1
		#Part	:PantheonGiantHull_Rune1.part
		#Part	:PantheonGiantHull_Rune2.part
#		BHVR	:GiantHullRunespin1.bhvr

#	End
#End

#Condition
#	On 	Time
#	Time 	30

#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegR_Node
#		AltPiv	1
		#Part	:PantheonGiantHull_Rune3.part
		#Part	:PantheonGiantHull_Rune4.part
#		BHVR	:GiantHullRunespin2.bhvr
	
#	End
#End

#Condition
#	On 	Time
#	Time 	70

#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegR_Node
#		AltPiv	1
		#Part	:PantheonGiantHull_Rune1.part
		#Part	:PantheonGiantHull_Rune4.part
#		BHVR	:GiantHullRunespin3.bhvr
	
#	End

#End


#########################################################
###### ULegL Runic Glow  #####################################
#########################################################

#Condition
#	On 	Time  
#	Time 	0
	
#	Event
#		EName 	Pantheon_UlegL_Node
#		Type	Local 
#		At	ULegL
#		GEOM	Pantheon_UlegL_Node
	
#	End

#End

#Condition
#	On 	Time
#	Time 	0
	
#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegL_Node
#		AltPiv	1
#		Part	:PantheonGiantHull_Rune1.part
#		Part	:PantheonGiantHull_Rune2.part
#		BHVR	:GiantHullRunespin1.bhvr

#	End
#End

#Condition
#	On 	Time
#	Time 	30

#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegL_Node
#		AltPiv	1
#		Part	:PantheonGiantHull_Rune3.part
#		Part	:PantheonGiantHull_Rune4.part
#		BHVR	:GiantHullRunespin2.bhvr
	
#	End
#End

#Condition
#	On 	Time
#	Time 	70

#	Event
#		EName 	LLegRRune1
#		Type	Local 
#		At	Pantheon_UlegL_Node
#		AltPiv	1
#		Part	:PantheonGiantHull_Rune1.part
#		Part	:PantheonGiantHull_Rune4.part
#		BHVR	:GiantHullRunespin3.bhvr
	
#	End

#End


#########################################################
###### KILL FX  #####################################
#########################################################

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	All
		Type	Destroy

	End

End

End


