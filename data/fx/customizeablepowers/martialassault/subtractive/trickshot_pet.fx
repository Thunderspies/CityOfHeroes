#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Hips
End

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	5

	Event
		EName	hookup
		Type	local
		At	Chest
	End

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		#altpiv	2
		Geom	GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			#Scale			2 2 2
			#Spin			0.0 -1.5 0
			InitialVelocity		0 0 0
			#InitialVelocityJitter	0.5 0.5 0
		End
		Part1	ENEMYFX\Tsoo\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

End
##################################

