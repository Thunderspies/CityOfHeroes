#########################################################
##	template

FxInfo

#########################################################
##Core
#############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	chest
		Part1	:EnergyCore.part
			Part	:EnergyCoreLightstreaks.part
			Part	:EnergyCoreGlowStreaks.part
			Part	:EnergyCoreLightStreaks2.part
		Part	:EnergyCoreSparks.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Sound	Energycore1_loop 50 30 .75
		
		LifeSpan 38
		
	End

#	Event
#		EName 	Prime
#		Type	Local 
#		At	chest
#		
#		Part	:EnergyCoreElectricity.part
#		Part	:EnergyCoreElectricityUP.part
#		Part	:EnergyCoreElectricityUPInvert.part
#		Part	:EnergyCoreElectricityDOWN.part
#		Part	:EnergyCoreElectricityDOWNInvert.part
##		Sound	Energycore4_loop 70 70 .85
#		
#		LifeSpan 38
#	End
#
############################################################
##Atoms
##############################################

	Event
		Ename	Monkey
		Type	Local
		At	chest
		Bhvr	:ElectronScale.bhvr
		Anim	FX_dark2
		
		LifeSpan 60
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey
		 AnimPiv Chest
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey
		 AnimPiv Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey
		 AnimPiv Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End
###############################################
	Event
		Ename	Monkey2
		Type	Local
		At	chest
		Bhvr	:ElectronScale2.bhvr
		Anim	FX_dark2
		
		LifeSpan 60
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey2
		 AnimPiv Chest
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey2
		 AnimPiv Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey2
		 AnimPiv Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End
###############################################
	Event
		Ename	Monkey3
		Type	Local
		At	chest
		Bhvr	:ElectronScale3.bhvr
		Anim	FX_dark2
		
		LifeSpan 60
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey3
		 AnimPiv Chest
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey3
		 AnimPiv Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey3
		 AnimPiv Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		
		LifeSpan 38

	End

End

######################################################3

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim2 60 60 .6
		End
end




Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		#Sound	clawYellow3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

Condition
	On Time 
	Time 10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks2.part
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 13

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 75

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	