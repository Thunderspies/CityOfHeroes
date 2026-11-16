#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################


Lifespan	440


											#
										       ###
											#
											#
										 #AUDIO	#
#########################################################################################

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Beam_Thin_01a.part
		Part	:Build_Up_Beam_Thin_01b.part
		Lifespan	200
	End	
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Beam_Core.part
		Part	:Build_Up_Beam_CoreFlat.part
		Lifespan	200
	End	
End


Condition
	On 	Time
	Time 	60	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Scatter_Beams_01.part
		Lifespan	270
	End	
End

Condition
	On 	Time
	Time 	200	
	
	Event
		Type	Start
		At	Root
		Bhvr	EnemyFx/DevouringEarth/Hamidon/Nucleus_Death/Electron_Camera_Shake.bhvr
		#Sound	rumble1_loop 200 80 .9
	End	
End


Condition
	On 	Time
	Time 	190	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Beam_Thick_01a.part
		Part	:Build_Up_Beam_Thick_01b.part
		Lifespan	160
	End	
End

Condition
	On 	Time
	Time 	175	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Scatter_Glows_01.part
		Lifespan	100
	End	
End

Condition
	On 	Time
	Time 	190	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Build_Up_Scatter_Beams_02.part
		Lifespan	70
	End	
End

Condition
	On 	Time
	Time 	200	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Death_Flash_Huge_01.part
		Lifespan	100
	End	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Death_Flash_Huge_02.part
		Lifespan	100
	End	
End

