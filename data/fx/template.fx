#########################################################
##	template
##  I think this has an example of every command

FxInfo

Flags SoundOnly InheritAlpha IsArmor InheritAnimScale

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		Bhvr	behaviors/_woomer/woomer.bhvr
		Geom	GEO_WepR_Chaingun_1
		Magnet	Target
		LookAt	Target
		At	HandR
		Part1	_woomer/cylinder.part
		Part2	_woomer/cylinder.part
		Part3	_woomer/cylinder.part
		PMagnet Target
		POther	Target
#	Other fields avaiable (search for examples?)
#		AltPiv	string
#		AnimPiv	string
#		SetState	string
#		Splat Texture0 Texture1
#		Sound Name Radius Fade Volume
#		While Bits
#		Until Bits
#		Debris
#		WorldGroup	string
#		Flags	AdoptParentEntity NoReticle PowerLoopingSound
	End

	Event
		EName 	Head
		Type	Local 
		Anim	bolt
		At	Head
		ChildFx woomer.fx
	End

	Event
		EName 	Prime
		Type	Local 
		At	HandR
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

End	


Condition
	On 	Time
	Time	100

	Event
		EName 	All
		Type	Destroy
	End

End	

End		