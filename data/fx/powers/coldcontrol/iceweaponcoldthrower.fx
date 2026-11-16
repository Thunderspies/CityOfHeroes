#########################################################
##	IceBolt
############################
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


LifeSpan	165

##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local 
		At	WepR
		Geom	energyShotgun
				
	End
	
End


Condition
	On 	Time
	Time 	24

	Event
		Type	local 
		At	WepR
		Sound Shotguncharge2 70 70 .6
				
	End
	
End


Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime
		Type	Start 
		At	Hookup
		Altpiv	1
		Bhvr	behaviors/powers/coldcontrol/iceWeaponblast2.bhvr
		#Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Sound Laserblast3 70 70 .8
		#Part2	:IceWeaponSnowBlast.part
		
		
	End
	
End

Condition
	On 	Time
	Time 	42.2

	Event
		EName 	Tracer2
		Type	local 
		At	Hookup
		Altpiv	1
		#Bhvr	behaviors/powers/coldcontrol/iceWeaponblast.bhvr
		#Geom	FX_IceBlast
		#Magnet	Target
		#LookAt	Target
		Part3	:IceThrowerSnow.part
		Part4	:IceThrowerMist1.part
		Part5	:IceThrowerMist2.part
		#Sound	lasericeshotgun 70 60 .74
		
	End
	
End

Condition
	On 	Time
	Time 	48

	Event
		Type	local 
		At	WepR
		Sound lasericeshotgun 70 70 .74
				
	End
	
End
###########################
Condition
	On 	Time
	Time 	0



	Event
		EName 	fist
		Type	Local 
		At	origin

		#Sound	Ice9 100.0 30.0 .75

	End


End


##############################################################


Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

#	Event	
#		Ename   IceShards
#		Type	Posit
#		At	target
#		Part1	:IceBoltMist.part
#		Part2	:ChillTouchSnow.part
#		Part3   :ChillingTouchFlash.part
#
#		LifeSpan	20
#
#	End

End


End				