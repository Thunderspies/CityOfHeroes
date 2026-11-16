#########################################################
##	
##

FxInfo

Lifespan	50

##################################
Condition
	On 	Time
	Time 	0

	Event
		Ename	Gun1
		Type	local
		At	Chest
		
		BHVR	:QuadTurretMachineGun1.bhvr
		Geom	TurretMachineGuns
		
	
	End

	Event
		Ename	Gun2
		Type	local
		At	Chest
		
		BHVR	:QuadTurretMachineGun2.bhvr
		Geom	TurretMachineGuns
		
	
	End

	Event
		Type	local
		At	Gun1
		altpiv	1
		
		Part	:TurrentMachinegunSmoke.part

		LifeSpan	2
	
	End

	Event
		Type	local
		At	Gun2
		altpiv	1
		Part	:TurrentMachinegunSmoke.part			
	
		LifeSpan	2

	End

End

Condition
	On 	cycle
	Time 	3
	#Chance	1

	Event
		Type	local
		At	Gun1
		altpiv	1
		BHVR	:TurretMachineGun.bhvr
		Geom	FX_MachineGunBurst
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Part	:TurrentMachinegunSmoke.part	
		Part	:TurrentMuzzflashSmall.part	

		LifeSpan	2
	
	End

	Event
		Type	local
		At	Gun2
		altpiv	1
		BHVR	:TurretMachineGun.bhvr
		Geom	FX_MachineGunBurst
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Part	:TurrentMuzzflashSmall.part
		Part	:TurrentMachinegunSmoke.part				
	
		LifeSpan	2

	End

	
End


End				