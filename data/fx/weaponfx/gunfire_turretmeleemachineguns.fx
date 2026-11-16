#########################################################
##	
##

FxInfo

Lifespan	200

##################################
Condition
	On 	Time
	Time 	0

	Event
		Ename	Gun1
		Type	local
		At	Chest
		
		BHVR	:TurretMachineGun1.bhvr
		Geom	TurretMachineGuns
		
	
	End

	Event
		Ename	Gun2
		Type	local
		At	Chest
		
		BHVR	:TurretMachineGun2.bhvr
		Geom	TurretMachineGuns
		
	
	End

	Event
		Type	local
		At	Gun1
		altpiv	1
		BHVR	:TurretMachineGun.bhvr
		Geom	FX_MachineGunBurst
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Part	:TurrentMachinegunSmoke.part	
		Part	:Gunfire_ShotgunMuzzflashSmall.part	

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
		Part	:Gunfire_ShotgunMuzzflashSmall.part
		Part	:TurrentMachinegunSmoke.part				
	
		LifeSpan	2

	End

	Event
		Type	local
		At	Gun1
		altpiv	1
		
		Part	:TurrentMachinegunSmoke.part

		LifeSpan	20
	
	End

	Event
		Type	local
		At	Gun2
		altpiv	1
		Part	:TurrentMachinegunSmoke.part			
	
		LifeSpan	20

	End
End


End				