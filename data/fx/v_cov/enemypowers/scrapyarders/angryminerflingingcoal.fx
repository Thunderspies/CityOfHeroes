#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	hold
		Type	local
		At	WepR
		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05
		Bhvr	:CoalFade.bhvr		
			
	End

	Event

		EName	Trail1
		Type	Local
		At	hold
		Part	:CoalDustTrail.part
		Part	:CoalDustRocks.part
		
	End

End



Condition
	On 	Time
	Time 	40

	Event
		EName	Prime
		Type	start
		At	Hold
		geom	parent
		Bhvr	:CoalProjectile.bhvr		
		Magnet	Target
		LookAt	Target
		Sound coalthrow 70 70 .44
			
	End

	Event

		EName	Trail2
		Type	Local
		At	Prime
		Part	:CoalDustTrail.part
		Part	:CoalDustRocks.part
		
	End

	Event

		EName	Hold
		Type	Destroy
		
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

