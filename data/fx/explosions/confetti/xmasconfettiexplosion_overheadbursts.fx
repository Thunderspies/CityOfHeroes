
FxInfo

Lifespan	120

########################################################################
############################## AUDIO ###################################
########################################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	Origin
		Sound confettiBlast 25 25 .4
	End
End




##################################################################
##Start fx
#######################################

Condition
	On 	Time
	Time 	30

	Event
		Ename	Center
		Type	start
		At	t_Root

		BhvrOverride
			PositionOffset	0 23 0
		End

	End

	Event
		Ename	Prime
		Type	start
		At	WepR

		Bhvr	Behaviors/Projectile.bhvr

		Part	:ConfettiTossSparkles.part
		Part	:Confetti_Green.part
		Part	:Confetti_Red.part
		Part	:Confetti_Yellow.part
		Part	:ConfettiBurst2.part

		magnet	Center
		Lookat	Center
	End


End

Condition
	On 	PrimeHit

	Event
		Type	start
		At	Prime
#		Part1	:ConfettiBurst.part
#		Part1	:ConfettiBurst.part
		Part1	:ConfettiSparkles.part
		Part	:HitRaysSmallest.part
		Part1	:ExplosiveFlash2.part
		lifeSpan	1

	End

	Event
		Ename	Prime
		Type	Destroy
	End

End
################################################

Condition
	On 	Time
	Time 	50

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End

Condition
	On 	Time
	Time 	55

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End

Condition
	On 	Time
	Time 	68

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End

Condition
	On 	Time
	Time 	72

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End

Condition
	On 	Time
	Time 	85

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End


Condition
	On 	Time
	Time 	95

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End


Condition
	On 	Time
	Time 	107

	Event
#		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 23 0
			StartJitter	5 1 5
		End

		ChildFx	:XmasConfettiExplosion3.fx
	End


End

End