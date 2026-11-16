#########################################################
##	
##

FxInfo
LifeSpan  200

##################################
Condition
	On 	Time
	Time 	49
	
	Event
		Type	Local
		At 	origin
		#Sound	Arachnosbot_blast1 100 80 .8
	End
End



Condition
	On 	Time
	Time 	0 #38


	Event
		ENAME   cannon
		Type	local
		At	hair
		bhvr	:SmallBot_Blaster_off.bhvr

	End


	Event
		ename	genspot
		Type	local
		At	waist
		Sound ArachnosBot_servo3 120.0 120.0 1.0

		bhvr	:SmallBot_Blaster_Genspot.bhvr
	end

	Event
		ename	gens
		Type	local
		At	waist

		bhvr	:SmallBot_Blaster_Gens.bhvr
		part	:Smallbot_blaster_gensparksBIG.part
	end

	Event
		Type	local
		At	cannon
		BhvrOverride
			PositionOffset		0 -.18 0
		End

		part	:Smallbot_blaster_chargeball.part
		lifespan 55
	end


	
	Event
		EName 	suctionR
		Type	local
		At	waist
		bhvr	:SmallBot_Blaster_BubbleOff.bhvr
		Lifespan 28
		PMagnet	waist
	End	
	Event
		EName 	suctionL
		Type	local
		At	waist
		bhvr	:SmallBot_Blaster_BubbleOffN.bhvr
		Lifespan 28
		PMagnet	waist
	End

	
	Event
		Type	local
		At	suctionR

##Core
		Part	:SmallBot_Blaster_BubblesBlue.part
		Part	:SmallBot_Blaster_suckingIn.part

##Suckin

		Part	:SmallBot_Blaster_BubblesTeal.part
		Part1	:SmallBot_Blaster_BubblessuckingIn.part
		Part2	:Smallbot_blaster_BubblesWhite.part
		Part	:SmallBot_Blaster_StringsWhite.part
		PMagnet	waist
	End
	
	Event
		Type	local
		At	suctionL

##Core
		Part	:SmallBot_Blaster_BubblesBlue.part
		Part	:SmallBot_Blaster_suckingIn.part

##Suckin
		Part	:SmallBot_Blaster_BubblesTeal.part
		Part1	:SmallBot_Blaster_BubblessuckingIn.part
		Part2	:Smallbot_blaster_BubblesWhite.part
		Part	:SmallBot_Blaster_StringsWhite.part
		PMagnet	waist
	End

end

Condition
	On 	time
	Time 	38

	Event
		ENAME   prime
		Type	start
		At	cannon
		

		BhvrOverride
			
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End


		bhvr	:SmallBot_Blaster.bhvr
		Part1	:SmallBot_Blaster_BallTailBig.part
		Part1	:SmallBot_Blaster_ShotBallGlow.part
		Part1	:SmallBot_Blaster_SteamTrail.part
		magnet	t_chest
		
	End


	Event
		ENAME   screw
		Type	local
		At	prime

		BhvrOverride
			
			Spin			0.0 0.0 2
			SpinJitter		0.0000 0.0000 0.4
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End

		bhvr	:SmallBot_Blaster_Twist.bhvr
	end


	Event
		Type	start
		At	prime
		
		Part1	:SmallBot_Blaster_ShotRing.part
		Part1	:SmallBot_Blaster_ShotRingGeneric.part
		Part	:SmallBot_Blaster_ShotRingThin.part
	end

	Event
		Type	local
		At	screw


		bhvr	:SmallBot_Blaster_TwistOff.bhvr
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		lifespan 60
	end
	
#	Event
#		Type	local
#		At	genspot
#		part1	:SmallBot_Blaster_ringSmoke.part
#		part1	:SmallBot_Blaster_ringSmokeN.part
#		part1	:SmallBot_Blaster_ringSteam.part
#		part1	:SmallBot_Blaster_ringSteamN.part
#		pmagnet root
#	end
#
End

Condition
	On 	time
	Time 	38

	Event
		Type	local
		At	screw

		BhvrOverride
			
			Spin			0.0 0.0 2
			SpinJitter		0.0000 0.0000 0.4
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End

		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		lifespan 60
	end

End	

Condition
	On 	time
	Time 	40

	Event
		Type	local
		At	screw

		BhvrOverride
			
			Spin			0.0 0.0 -1
			SpinJitter		0.0000 0.0000 0.4
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End

		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		lifespan 60
	end

End


Condition
	On 	time
	Time 	45

	Event
		Type	local
		At	screw

		BhvrOverride
			
			Spin			0.0 0.0 -2
			SpinJitter		0.0000 0.0000 0.4
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End

		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		lifespan 60
	end

End



Condition
	On 	time
	Time 	43

	Event
		Type	local
		At	screw

		BhvrOverride
			
			Spin			0.0 0.0 1
			SpinJitter		0.0000 0.0000 0.4
			Scale			1 1 1
			ScaleRate		1.2 1.2 1.2
			 EndScale		1 1 1
		End

		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		Part1	:SmallBot_Blaster_Twist_Shot.part
		lifespan 60
	end

End

Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
end
