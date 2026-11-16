#########################################################
##	tech_doorgates
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

######################################################3
##Core
#############################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		
		Part	:EnergyBalls.part
		Part	:Energy3rdLightHits.part
		Part	:Energy3rdBallHits.part

		Part1	:EnergyCore.part
			Part	:EnergyCoreLightstreaks.part
			Part	:EnergyCoreGlowStreaks.part
			Part	:EnergyCoreLightStreaks2.part
		Part	:EnergyCoreSparks.part
		Part	:EnergyCoreElectricity3rd.part

		Sound Energycore1_loop 80 80 .75
		
	End

	Event
		Type	Local 
		At	Origin
		
		Part	:EnergyCoreElectricity.part
		Part	:EnergyCoreElectricityUP.part
		Part	:EnergyCoreElectricityUPInvert.part
		Part	:EnergyCoreElectricityDOWN.part
		Part	:EnergyCoreElectricityDOWNInvert.part
		Sound Energycore4_loop 80 80 .3
	End

############################################################
##Atoms
##############################################

	Event
		Ename	Monkey
		Type	Local
		At	Origin
		Bhvr	Behaviors/ElectronScaleFasta.bhvr
		Anim	FX_dark2
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey
		 AnimPiv Chest
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey
		 AnimPiv Neck
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey
		 AnimPiv Waist
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End
###############################################
	Event
		Ename	Monkey2
		Type	Local
		At	Origin
		Bhvr	Behaviors/ElectronScaleFast02c.bhvr
		Anim	FX_dark2
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey2
		 AnimPiv Chest
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey2
		 AnimPiv Neck
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey2
		 AnimPiv Waist
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End
###############################################
	Event
		Ename	Monkey3
		Type	Local
		At	Origin
		Bhvr	Behaviors/ElectronScaleFast03c.bhvr
		Anim	FX_dark2
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey3
		 AnimPiv Chest
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey3
		 AnimPiv Neck
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey3
		 AnimPiv Waist
		Part1	:Electron2nd.part
		Part2	:ElectronLocal2nd.part

	End

End


End			