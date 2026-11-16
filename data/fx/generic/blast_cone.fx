#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 50


#########################################################
#################### BLAST 1	 ########################
#########################################################


### fake target 1 ######################################

Condition
	On	Time
	Time	10					## fate target should appear atleast on frame before projectile

	Event
		Ename	FakeTarget_1
		Type	local
		At	Head  ##Bendmystic
		BHVR	behaviors\target_offset.bhvr
		Geom	drum_small			## you don't actually need a Geom here
	End

End

#######  projectile 1  #################################

Condition
	On 	Time
	Time 	11
	
	Event
		EName 	prime
		Type	start
		At	Head
		Bhvr	WEAPONS\Bow\Archery\projectile.bhvr   ## make a new projectile bhvr - change speed and stretch
		Geom	Drum_big			 

		Magnet	FakeTarget_1
		Lookat	FakeTarget_1
		LifeSpan 10

	End

End	











End		