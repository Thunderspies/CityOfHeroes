#########################################################
##	
##

FxInfo
LifeSpan  70

##################################

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
		sound	QuadTurret 280.0 30.0 1.0
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
		
		Part	:50Cal_fireQuad.part
		##Part	:50Cal_firehead.part
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
	Time 	19

	Event
		Type	Start
		At	ToeL
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
		At	ToeL	
		Part	:Turrentsmoke.part
		#sound	shotgun3 120.0 30.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Prime1
		Type	Start
		At	ToeL
		
		Part	:50Cal_fireQuad.part
		##Part	:50Cal_firehead.part
		Magnet	Chest

	End

	Event
		EName 	tracer1
		Type	Start
		At	ToeL
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	ToeL
		
		Geom	Fx_GunBurst
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2

	End

	
End


##############################################################
##Round 3
####################################################

Condition
	On 	Time
	Time 	24

	Event
		Type	Start
		At	ToeR
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
		At	ToeR	
		Part	:Turrentsmoke.part
		#sound	shotgun3 120.0 30.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Prime2
		Type	Start
		At	ToeR
		
		Part	:50Cal_fireQuad.part
		##Part	:50Cal_firehead.part
		Magnet	Chest

	End

	Event
		EName 	tracer1
		Type	Start
		At	ToeR
		
		Geom	Fx_GunBurst
		
		Bhvr	:50CalTurentBlastOrient1.bhvr
		
		LifeSpan  2

	End

	Event
		EName 	tracer
		Type	Start
		At	ToeR
		
		Geom	Fx_GunBurst
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Bhvr	:50CalTurentBlastOrient.bhvr
		
		LifeSpan  2

	End

	
End

##############################################################
##Round 4
####################################################

Condition
	On 	Time
	Time 	29

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
		#sound	shotgun3 120.0 30.0 .9
		LifeSpan	2
		
	End

End

Condition
	On 	Time
	Time 	32


	Event
		EName 	Prime3
		Type	Start
		At	WepL
		
		Part	:50Cal_fireQuad.part
		##Part	:50Cal_firehead.part
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
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End

End

Condition
	On 	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End

End


Condition
	On 	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End

End

End				