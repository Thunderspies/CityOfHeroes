#########################################################
##	Fire_Ball
##
FxInfo


LifeSpan	460

Condition
	Event
		EName 	GunTip
		Type	local 
		At	WepR
		Bhvr	:Particle_Accelerator_AltPivot.bhvr
		Sound particlecannon2 100 100 .92
	End
End

################## Beam ##########################
Condition
	On 	Time
	Time 	39  ##22 

	Event
		EName 	Prime2
		Type	Posit 
		At	GunTip
		Bhvr	behaviors/Particle_Accelerator_Beam.bhvr 

		geom	Particle_Accelerator3

		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Prime3
		Type	Posit 
		At	GunTip
		Bhvr	behaviors/Particle_Accelerator_Small.bhvr
		
		geom	Particle_Accelerator4

		Magnet	T_Chest
		LookAt	T_Chest
	End
End

################# Pulsing Particles #####################
Condition
	Time 	39  ##22
	Event
		EName	prime
		Type	start
		At	GunTip
		Bhvr	Behaviors/Particle_Accelerator_Pulse.bhvr
		Part1	:Particle_Accelerator_04.part
		Part2	:Particle_Accelerator_05.part
		Part3	:Particle_Accelerator_06.part
		#LifeSpan 30
		
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
	
End
############################################################

############## Gun Spray ###################################
Condition
	Time 39  ##15
	Event
		Type	Posit
		At	GunTip
		Lookat	T_Chest
		Part	:Particle_Accelerator_01.part
		Part	:Particle_Accelerator_02.part
	End

	Event
		Type	Posit
		At	GunTip
		Lookat	T_Chest
		Part	:Particle_Accelerator_Glow_01.part
		Part	:Particle_Accelerator_Glow_02.part
	End
End
############################################################

############# Target Spray #################################
Condition
	On Primehit

	Event
		Type	Posit
		At	T_Chest
		Lookat	Origin
		Part	:Particle_Accelerator_01.part
		Part	:Particle_Accelerator_03.part
		Sound particlehit_loop 100 100 .7
	End
End
########################################################

End	


			