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
	Inpname	Hips
End

Input  
	Inpname	Root
End

Input  
	Inpname	Emblem
End

Input  
	Inpname	LArmR
End

Input  
	Inpname	LArmL
End

Input  
	Inpname	LLegR
End

Input  
	Inpname	LLegL
End

Flags    InheritAlpha

#########################################################
###### EYE GLOWS #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemNode
		Type	Local 
		At	Hips
		GEOM	TotemChestNode
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemEyes
		Type	Local 
		At	TotemNode
		AltPiv	6
		Part	:PantheonTotem_Glow4.part

	End
	Event
		EName 	TotemEyes
		Type	Local 
		At	TotemNode
		AltPiv	7
		Part	:PantheonTotem_Glow4.part

	End

End

#########################################################
###### CHEST GLOWS #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	1
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	2
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	3
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	4
		Part	:PantheonTotem_Glow1.part
	
	End

End

#########################################################
###### CHEST/BACK GLOW  #####################################
#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemChest1
		Type	Local 
		At	TotemNode
		AltPiv	5
		Part	:PantheonTotem_Glow2.part
		Part	:PantheonTotem_Glow3.part
	
	End

End

#########################################################
###### LOWER LIMB GLOWS #####################################
#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemNode1
		Type	Local 
		At	LArmR
		GEOM	TotemLArmRNode
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemNode2
		Type	Local 
		At	LArmL
		GEOM	TotemLArmLNode
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemNode3
		Type	Local 
		At	LLegR
		GEOM	TotemLLegRNode
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemNode4
		Type	Local 
		At	LLegL
		GEOM	TotemLLegLNode
	
	End

End

#########################################################
###### ARM GLOWS    #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode1
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode2
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode3
		Part	:PantheonTotem_Glow1.part
	
	End
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode4
		Part	:PantheonTotem_Glow1.part
	
	End

End

#########################################################
###### LArmR Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode1
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin4.bhvr
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode1
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode1
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin6.bhvr
	
	End

End

#########################################################
###### UArmR Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	1
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin4.bhvr
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	1
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	1
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin6.bhvr
	
	End

End

#########################################################
###### LArmL Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode2
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin4.bhvr
	
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode2
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode2
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin6.bhvr
	
	End

End

#########################################################
###### UArmL Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	4
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin4.bhvr
		Sound totem1_loop 46 46 .32
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	4
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin5.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode
		AltPiv	4
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin6.bhvr
	
	End

End

#########################################################
###### LLegR Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode3
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin1.bhvr
		Sound totem2_loop 56 56 .66
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode3
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin2.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode3
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin3.bhvr
	
	End

End

#########################################################
###### LLegL Runic Glow  #####################################
#########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode4
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune2.part
		BHVR	:Runespin1.bhvr
	
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode4
		Part	:PantheonTotem_Rune3.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin2.bhvr
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	TotemGlow1
		Type	Local 
		At	TotemNode4
		Part	:PantheonTotem_Rune1.part
		Part	:PantheonTotem_Rune4.part
		BHVR	:Runespin3.bhvr
	
	End

End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	All
		Type	Destroy

	End

End

End


