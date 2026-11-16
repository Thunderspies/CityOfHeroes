#########################################################
##	
##

FxInfo
LifeSpan  150

##################################

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	Mystic
		Part	:TurretBlast.part
		Part	:TurretBlast2.part
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2
	End

	Event
		EName 	tracer2
		Type	start
		At	Mystic	
		Part	:Turrentsmoke.part
		#sound	shotgun3 120.0 30.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName 	tracer1
		Type	Start
		At	Mystic
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	Mystic
		
		Geom	Fx_GunBurst
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2

	End

	
End

##############################################################
##Round 2
####################################################

Condition
	On 	Time
	Time 	27

	Event
		Type	Start
		At	mystic
		Part	:TurretBlast.part
		Part	:TurretBlast2.part
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2
	End

	Event
		EName 	tracer2
		Type	start
		At	mystic	
		Part	:Turrentsmoke.part
		#sound	shotgun3 120.0 30.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	tracer1
		Type	Start
		At	mystic
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	mystic
		
		Geom	Fx_GunBurst
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2

	End

	
End

End				