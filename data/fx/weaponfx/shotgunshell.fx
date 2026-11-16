#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	30


	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
		
	End

	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		#Part1	WEAPONFX/Gunfire_ShotgunShell.Part
		ChildFX	V_COV\PhysicsEnabled\ShotGunShellEject.fx
		Sound Shell1 100.0 100.0 .8
	End

End


End				