#########################################################
##	
##

FxInfo

LifeSpan  200

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
##################################
Condition
	On 	Time
	Time 	0

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	Sniper01
		
	End

	Event
		EName 	Blast1
		Type	start
		At	p2
		Altpiv 1
		
		Sound tranqdart 120.0 120.0 1.0
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		LifeSpan	4
	End

	Event
		EName 	Prime
		Type	start
		At	p2
		Altpiv 1
		Bhvr	Behaviors\projectile2Tranq.bhvr
		Geom	CrossBowBolt
		magnet	target
		lookat	target
	End

End

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End

End



End				