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

LifeSpan	180

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
	Time 	34

	Event
		Type	local 
		At	WepR
		Sound Shotguncharge2 70 70 .8
				
	End
	
End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime
		Type	Start 
		At	Hookup
		Altpiv	1
		Bhvr	behaviors/powers/coldcontrol/iceWeaponblast2.bhvr
		
		Magnet	Target
		LookAt	Target
		
		#Part2	:IceWeaponSnowBlast.part
		Sound Laserblast3 70 70 .8
		
		
	End
	
End

Condition
	On 	Time
	Time 	52.2

	Event
		EName 	tracer2
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
	Time 	58

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