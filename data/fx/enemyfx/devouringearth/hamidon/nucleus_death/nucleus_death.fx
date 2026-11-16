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

Input  
	Inpname	Waist
End

Flags    InheritAlpha

#########################################################


Lifespan	530

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound Hamidondeath2 4000 4000 .97
	End
End

Condition
	Event
		EName	DummyChest
		Type	Local
		At	Chest
	End

	Event
		EName	DummyHips
		Type	Local
		At	Hips
		
	End
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Beam_Thin_01a.part
		Part	:Build_Up_Beam_Thin_01b.part
		Lifespan	440
	End	
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Beam_Core.part
	End	
End


Condition
	On 	Time
	Time 	80	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Scatter_Beams_01.part
		Lifespan	370
	End	
End

Condition
	On 	Time
	Time 	200	
	
	Event
		Type	Start
		At	Waist
		Bhvr	:Nucleus_Camera_Shake.bhvr
		Sound rumble1_loop 200 200 .9
	End	
End


Condition
	On 	Time
	Time 	200	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Beam_Thick_01a.part
		Part	:Build_Up_Beam_Thick_01b.part
		Lifespan	250
	End	
End

Condition
	On 	Time
	Time 	210	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Left.part
		Part	:Nucleus_Gut_Burst_Left_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	250	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Right.part
		Part	:Nucleus_Gut_Burst_Right_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	290	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Top.part
		Part	:Nucleus_Gut_Burst_Top_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	310	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Left.part
		Part	:Nucleus_Gut_Burst_Left_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	340	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Right.part
		Part	:Nucleus_Gut_Burst_Right_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	365	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Top.part
		Part	:Nucleus_Gut_Burst_Top_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	360	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Right.part
		Part	:Nucleus_Gut_Burst_Right_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	385	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Nucleus_Gut_Burst_Top.part
		Part	:Nucleus_Gut_Burst_Top_Small.part
		Lifespan	7
	End	
End

Condition
	On 	Time
	Time 	380	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Scatter_Glows_01.part
		Lifespan	100
	End	
End

Condition
	On 	Time
	Time 	410	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Build_Up_Scatter_Beams_02.part
		Lifespan	70
	End	
End

Condition
	On 	Time
	Time 	510	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Death_Flash_Huge_01.part
		Lifespan	50
	End	
End

Condition
	On 	Time
	Time 	510	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Death_Flash_Huge_02.part
		Lifespan	50
	End	
End

