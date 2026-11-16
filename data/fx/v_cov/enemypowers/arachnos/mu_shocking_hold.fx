#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

Input
	Inpname	Origin
End

Input
	Inpname	Target
End

Input
	Inpname	Mystic
End

Input
	Inpname	HandR
End

#LifeSpan	40

########################################################



Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At 	origin
		Sound	elecattack4 80 80 .86
	End
End



Condition

	On 	cycle
	Time 	4
#	Event
#		Type	Start #local
#
#		At	UarmR
#		part1	:mu_Elec_BodyShockDown1.part
#		part2	:mu_Elec_BodyShock1.part
#		part2	:mu_Elec_BodyShock2.part
#		Pother LarmR
#		pmagnet GroundR #UarmR
#		lifespan 10
#	end
#
#	Event
#		Type	local
#
#		At	LarmR
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother HandR
#		pmagnet GroundR #UarmR
#		lifespan 10
#	end
#
#	Event
#		Type	Start #local
#
#		At	UarmL
#		part1	:mu_Elec_BodyShockDown1.part
#		part2	:mu_Elec_BodyShock1.part
#		part2	:mu_Elec_BodyShock2.part
#		Pother LarmL
#		pmagnet GroundL #UarmR
#		lifespan 10
#	end
#
#	Event
#		Type	Start #local
#
#		At	LarmL
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother HandL
#		pmagnet GroundL #UarmR
#		lifespan 10
#	end
#
#	Event
#		Type	Start #local
#
#		At	UlegR
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother LlegR
#		pmagnet GroundR #UarmR
#		lifespan 10
#	end

#	Event
#		Type	Start #local
#
#		At	LlegR
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother FootR
#		pmagnet GroundR #UarmR
#		lifespan 10
#	end

#	Event
#		Type	Start #local
#
#		At	UlegL
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother LlegL
#		pmagnet GroundL #UarmR
#		lifespan 10
#	end

#	Event
#		Type	Start #local
#
#		At	LlegL
#		part1	:mu_Elec_BodyShockDown1.part
#		Pother FootL
#		pmagnet GroundL #UarmR
#		lifespan 10
#	end

end



Condition

	On 	cycle
	Time 	4

	Event
		Ename	ArmR
		Type	Local

		At	Root
		bhvr	:mu_ShockHold_GroundR.bhvr
		Lifespan	1
	end
	Event
		Type	positonly

		At	HandR
		part1	:mu_ShockHold_Ground.part
		Pother ArmR
		pmagnet HandR
		Lifespan	40
	end


	Event
		Ename	ArmL
		Type	local

		At	Root
		bhvr	:mu_ShockHold_GroundL.bhvr
		Lifespan	1
	end
	Event
		Type	positonly

		At	HandL
		part1	:mu_ShockHold_Ground.part
		Pother ArmL
		pmagnet HandL
		Lifespan	20
	end



end


