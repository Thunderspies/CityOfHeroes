#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End 

Flags    InheritAlpha


##################################
Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime
		Type	start 
		At	WepR
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyShotgunBullet.part
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	Blast
		Type	start
		At	WepR
		Geom	LaserAssault
		
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		Part1	WEAPONFX/Energy_ShotgunMuzzflash.part
		Part2	WEAPONFX/EnergyShotGunFlash.part
		#Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

End

##################################

Condition
	On 	Time
	Time	32

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	37

	Event
		Ename 	tracer
		Type	Destroy
	End
End


End				