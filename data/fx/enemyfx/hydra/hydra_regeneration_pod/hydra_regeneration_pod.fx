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
	Inpname	Head
End

Input  
	Inpname	Eyes
End

Input  
	Inpname	Root
End

Input  
	Inpname	Chest
End

Flags    InheritAlpha

#########################################################




Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hydra_Pod_Node
		Type	local
		At	Hips
		Geom	Hydra_Regeneration_Pod_Node
		Sound hydrapod1_loop 70 70 .64
	End

	Event
		EName 	Hamidon
		Type	local
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Inner_Pulse_01.part
		Part	:Hydra_Pod_Inner_Pulse_02.part
		Part	:Hydra_Pod_Outer_Shield.part
		Part	:Hydra_Pod_Beams.part
		#Part	:Hydra_Pod_Inward_Specs.part
		#Part	:Hydra_Pod_Cloud1.part
		Part	:Hydra_Pod_Upper_Jet_01.part
		Part	:Hydra_Pod_Downward_Jet_01.part
		Sound hydrapod2_loop 70 70 .78
	End	

	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin1.bhvr
		
	End
	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin2.bhvr
		
	End
	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin3.bhvr
		
	End

	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin4.bhvr
		
	End
	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin5.bhvr
		
	End
	Event
		EName 	Hamidon
		Type	Local 
		At	Hydra_Pod_Node
		AltPiv	1
		Part	:Hydra_Pod_Spin.part
		BHVR	:Podspin6.bhvr
		
	End
End
