#########################################################
##	
##

FxInfo
LifeSpan  150

##################################

Condition
	Event
		Type Local
		At Origin
		Sound Arachnos_DualTurret_Servo_01 100 100 .1
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	WepR
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
		At	WepR	
		Part	:Turrentsmoke.part
		SoundNoRepeat 3
		sound	Arachnos_DualTurret_01 260.0 130.0 .9
		sound	Arachnos_DualTurret_02 260.0 130.0 .9
		sound	Arachnos_DualTurret_03 260.0 130.0 .9
		sound	Arachnos_DualTurret_04 260.0 130.0 .9
		sound	Arachnos_DualTurret_05 260.0 130.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepR
		#Part	:50Cal_firehead.part
		Part	:50Cal_fire.part
		#Bhvr	:50CalTracer.bhvr
		Magnet	Chest

	End

	Event
		EName 	tracer1
		Type	Start
		At	WepR
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	WepR
		
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
		At	WepL
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
		At	WepL	
		Part	:Turrentsmoke.part
		SoundNoRepeat 3
		sound	Arachnos_DualTurret_01 260.0 130.0 .9
		sound	Arachnos_DualTurret_02 260.0 130.0 .9
		sound	Arachnos_DualTurret_03 260.0 130.0 .9
		sound	Arachnos_DualTurret_04 260.0 130.0 .9
		sound	Arachnos_DualTurret_05 260.0 130.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	30
	
	
	Event
		EName 	Prime1
		Type	Start
		At	WepL
		
		Part	:50Cal_fire.part
		#Part	:50Cal_firehead.part
		#Bhvr	:50CalTurentBlastOrient1.bhvr
		Magnet	Chest

	End

	Event
		EName 	tracer1
		Type	Start
		At	WepL
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	WepL
		
		Geom	Fx_GunBurst
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2

	End

	
End


Condition
	On 	Primehit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1hit
	
	Event
		Ename 	Prime1
		Type	Destroy
	End

End

End				